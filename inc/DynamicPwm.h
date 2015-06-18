#include "mbed.h"

#ifndef MICROBIT_SMART_PWM_BUS_H
#define MICROBIT_SMART_PWM_BUS_H

#define NO_PWMS 3

enum PwmPersistence
{
    PWM_PERSISTENCE_TRANSIENT = 1,
    PWM_PERSISTENCE_PERSISTENT = 2,
};

/**
  * Class definition for DynamicPwm.
  *
  * This class addresses a few issues found in the underlying libraries. 
  * This provides the ability for a neat, clean swap between PWM channels.
  */
class DynamicPwm : public PwmOut
{
    static DynamicPwm* pwms[NO_PWMS];
    uint8_t flags;
    
    /**
      * An internal constructor used when allocating a new DynamicPwm representation
      * @param pin the name of the pin for the pwm to target
      * @param persistance the level of persistence for this pin PWM_PERSISTENCE_PERSISTENT (can not be replaced until freed, should only be used for system services really.) 
      * or PWM_PERSISTENCE_TRANSIENT (can be replaced at any point if a channel is required.)
      */
    DynamicPwm(PinName pin, PwmPersistence persistence = PWM_PERSISTENCE_TRANSIENT);
    
    public: 
    
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
    void redirect(PinName pin);
    

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
    static DynamicPwm* allocate(PinName pin, PwmPersistence persistence = PWM_PERSISTENCE_TRANSIENT);
    
    /**
      * Frees this DynamicPwm instance if the pointer is valid.
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate();
      * pwm->free();
      * @endcode
      */
    void free();
    
    /**
      * Retreives the pin name associated with this DynamicPwm instance.
      *
      * Example:
      * @code
      * DynamicPwm* pwm = DynamicPwm::allocate(PinName n);
      * pwm->getPinName(); // equal to n
      * @endcode
      */
    PinName getPinName();
    
};

#endif