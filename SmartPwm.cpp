#include "SmartPwm.h"


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

SmartPwm::SmartPwm(PinName pin) : PwmOut(pin)
{
}

void SmartPwm::redirect(PinName pin)
{   
    gpiote_reinit(pin, _pwm.pin, (uint8_t) _pwm.pwm);  
    _pwm.pin = pin;
}