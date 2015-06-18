#include "SmartPwm.h"


SmartPwm::SmartPwm(PinName pin) : PwmOut(pin)
{
}

SmartPwm::~SmartPwm(){
    
    pwmout_free(&_pwm); 
    NRF_GPIOTE->CONFIG[(uint8_t) _pwm.pwm] = 0;  
}

void SmartPwm::redirect(PinName pin)
{   
    //gpiote_reinit(pin, _pwm.pin, (uint8_t) _pwm.pwm);  
    _pwm.pin = pin;
}