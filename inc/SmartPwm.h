#include "mbed.h"

#ifndef MICROBIT_SMART_PWM_BUS_H
#define MICROBIT_SMART_PWM_BUS_H

/**
  * Class definition for SmartPwm.
  * 
  * This class addresses a few issues found in the underlying libraries. 
  * This provides the ability for a neat, clean swap between PWM channels.
  *
  * @note **DEPRACATED** Replaced by DynamicPwm - will be factored out soon! 
  */
class SmartPwm : public PwmOut
{
    public: 
    
    /**
      * Simply wraps the PwmOut constructor, and sets the resulting instance as a property.
      * @param pin The name of the pin to direct PWM at.
      */
    SmartPwm(PinName pin);
    
    /**
      * Neatly redirects the PWM channel to the given pin.
      * @param pin the new pin to direct PWM at!
      */
    void redirect(PinName pin);
    
    /**
      * This destructor completely disables PWM (we found that mbed doesn't completely disable the current channel).
      */
    ~SmartPwm();
};

#endif