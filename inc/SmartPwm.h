#include "mbed.h"

#ifndef MICROBIT_SMART_PWM_BUS_H
#define MICROBIT_SMART_PWM_BUS_H

class SmartPwm : public PwmOut
{
    public: 
    
    SmartPwm(PinName pin);
    
    void redirect(PinName pin);
    
    ~SmartPwm();
};

#endif