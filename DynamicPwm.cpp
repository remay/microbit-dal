#include "DynamicPwm.h"


DynamicPwm* DynamicPwm::pwms[NO_PWMS] = { NULL };

/**
  * Reassigns an already operational PWM channel to the given pin
  * #HACK #BODGE # YUCK #MBED_SHOULD_DO_THIS
  *
  * @param pin The pin to start running PWM on
  * @param oldPin The pin to stop running PWM on
  * @param channel_number The GPIOTE channel being used to drive this PWM channel
  */  
void gpiote_reinit(PinName pin, PinName oldPin, uint8_t channel_number)
{        
    NRF_TIMER2->EVENTS_COMPARE[3] = 0;
    NRF_TIMER2->TASKS_STOP = 1;

    // Connect GPIO input buffers and configure PWM_OUTPUT_PIN_NUMBER as an output.
    NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                            | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                            | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                            | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                            | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);

    NRF_GPIO->OUTCLR = (1 << oldPin);
    NRF_GPIO->OUTCLR = (1 << pin);

    /* Finally configure the channel as the caller expects. If OUTINIT works, the channel is configured properly.
       If it does not, the channel output inheritance sets the proper level. */

        NRF_GPIOTE->CONFIG[channel_number] = (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos) |
                                         ((uint32_t)pin << GPIOTE_CONFIG_PSEL_Pos) |
                                         ((uint32_t)GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) |
                                         ((uint32_t)GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos); // ((uint32_t)GPIOTE_CONFIG_OUTINIT_High <<
                                                                                                             // GPIOTE_CONFIG_OUTINIT_Pos);//

    /* Three NOPs are required to make sure configuration is written before setting tasks or getting events */
    __NOP();
    __NOP();
    __NOP();
    
    NRF_TIMER2->CC[0] = 0;
    NRF_TIMER2->CC[1] = 0;    
    NRF_TIMER2->EVENTS_COMPARE[3] = 0;
    
    NRF_TIMER2->TASKS_START        = 1;
}

/**
  * An internal constructor used when allocating a new DynamicPwm representation
  * @param pin the name of the pin for the pwm to target
  * @param persistance the level of persistence for this pin PWM_PERSISTENCE_PERSISTENT (can not be replaced until freed, should only be used for system services really.) 
  * or PWM_PERSISTENCE_TRANSIENT (can be replaced at any point if a channel is required.)
  */
DynamicPwm::DynamicPwm(PinName pin, PwmPersistence persistence) : PwmOut(pin)
{
    this->flags = persistence;
}

/**
  * Redirects the pwm channel to point at a different pin.
  * @param pin the new pin to direct PWM at.
  *
  * Example:
  * @code
  * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
  * pwm->redirect(PinName n2); // pwm is now produced on n2
  * @endcode
  */
void DynamicPwm::redirect(PinName pin)
{   
    gpiote_reinit(pin, _pwm.pin, (uint8_t)_pwm.pwm);  
    this->_pwm.pin = pin;
}

/**
  * Retrieves a pointer to the first available free pwm channel - or the first one that can be reallocated.
  * @param pin the name of the pin for the pwm to target
  * @param persistance the level of persistence for this pin PWM_PERSISTENCE_PERSISTENT (can not be replaced until freed, should only be used for system services really.) 
  * or PWM_PERSISTENCE_TRANSIENT (can be replaced at any point if a channel is required.)
  *
  * Example:
  * @code
  * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
  * @endcode
  */
DynamicPwm* DynamicPwm::allocate(PinName pin, PwmPersistence persistence)
{
    //try to find a blank spot first
    for(int i = 0; i < NO_PWMS; i++)
    {
        if(pwms[i] == NULL)
        {
            pwms[i] = new DynamicPwm(pin, persistence);
            return pwms[i];
        }   
    }
    
    //no blank spot.. try to find a transient PWM
    for(int i = 0; i < NO_PWMS; i++)
    {
        if(pwms[i]->flags & PWM_PERSISTENCE_TRANSIENT)
        {
            pwms[i]->flags = persistence;
            pwms[i]->redirect(pin);
            return pwms[i];
        }   
    }
    
    return (DynamicPwm*)NULL;
}

/**
  * Frees this DynamicPwm instance if the pointer is valid.
  *
  * Example:
  * @code
  * DynamicPwm* pwm = DynamicPwm::allocate();
  * pwm->free();
  * @endcode
  */
void DynamicPwm::free()
{
    
    //free the pwm instance.
    NRF_GPIOTE->CONFIG[(uint8_t) _pwm.pwm] = 0;
    pwmout_free(&_pwm); 
    this->flags = PWM_PERSISTENCE_TRANSIENT;
    
    //set the pointer to this object to null...
    for(int i =0; i < NO_PWMS; i++)
        if(pwms[i] == this)
        {
            delete pwms[i];
            pwms[i] = NULL;
        }
}

/**
  * Retreives the pin name associated with this DynamicPwm instance.
  *
  * Example:
  * @code
  * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
  * pwm->getPinName(); // equal to n
  * @endcode
  */
PinName DynamicPwm::getPinName()
{
    return _pwm.pin;   
}