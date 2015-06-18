#include "MicroBit.h"
#include "inc/MicroBitPin.h"

#define MICROBIT_PIN_MAX_OUTPUT 255

/**
  * Constructor. 
  * Create a Button representation with the given ID.
  * @param id the ID of the new Pin object.
  * @param name the pin name for this MicroBitPin instance to represent
  * @param capability the capability of this pin, can it only be digital? can it only be analog? can it be both?
  * 
  * Example:
  * @code 
  * MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_BOTH);
  * @endcode
  */
MicroBitPin::MicroBitPin(int id, PinName name, PinCapability capability) 
{   
    //set mandatory attributes 
    this->id = id;
    this->name = name;
    this->capability = capability;
    
    // Power up in a disconnected, low power state.
    // If we're unused, this is how it will stay...
    this->status = 0x00;
    this->pin = NULL;
    
}

/**
  * Disconnect any attached mBed IO from this pin.
  * Used only when pin changes mode (i.e. Input/Output/Analog/Digital)
  */
void MicroBitPin::disconnect()
{
    // This is a bit ugly, but rarely used code.
    // It would be much better to use some polymorphism here, but the mBed I/O classes aren't arranged in an inheritance hierarchy.
    if (status & IO_STATUS_DIGITAL_IN)
        delete ((DigitalIn *)pin);

    if (status & IO_STATUS_DIGITAL_OUT)
        delete ((DigitalOut *)pin);

    if (status & IO_STATUS_ANALOG_IN){
        NRF_ADC->ENABLE = ADC_ENABLE_ENABLE_Disabled; // forcibly disable the ADC - BUG IN EMBED....
        delete ((AnalogIn *)pin);
    }
    
    if (status & IO_STATUS_ANALOG_OUT)
    {
        if(((DynamicPwm *)pin)->getPinName() == name)
            ((DynamicPwm *)pin)->free(); 
    }   
    
    this->pin = NULL;
    this->status = status & IO_STATUS_EVENTBUS_ENABLED; //retain event bus status
}

/**
  * Configures this IO pin as a digital output (if necessary) and sets the pin to 'value'.
  * @param value 0 (LO) or 1 (HI)
  * 
  * Example:
  * @code 
  * MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_BOTH);
  * P0.setDigitalValue(1); // P0 is now HI!
  * @endcode
  */
void MicroBitPin::setDigitalValue(int value)
{
    //check if this pin has a digital mode...
    if(!(PIN_CAPABILITY_DIGITAL & capability) || value < 0 || value > 1)
        return;
        
    // Move into a Digital input state if necessary.
    if (!(status & IO_STATUS_DIGITAL_OUT)){
        disconnect();  
        pin = new DigitalOut(name);
        status |= IO_STATUS_DIGITAL_OUT;
    }
    
    //write the value!
    ((DigitalOut *)pin)->write(value);
}

/**
  * Configures this IO pin as a digital input (if necessary) and tests its current value.
  * @return 1 if this input is high, 0 otherwise.
  * 
  * Example:
  * @code 
  * MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_BOTH);
  * P0.getDigitalValue(); // P0 is either 0 or 1;
  * @endcode
  */
int MicroBitPin::getDigitalValue()
{
    //check if this pin has a digital mode...
    if(!(PIN_CAPABILITY_DIGITAL & capability))
        return MICROBIT_IO_OP_NA;
    
    // Move into a Digital input state if necessary.
    if (!(status & IO_STATUS_DIGITAL_IN)){
        disconnect();  
        pin = new DigitalIn(name,PullDown); //pull down!
        status |= IO_STATUS_DIGITAL_IN;
    }
    
    return ((DigitalIn *)pin)->read();
}

/**
  * Configures this IO pin as an analogue output (if necessary and possible).
  * Change the DAC value to the given level.
  * @param value the level to set on the output pin, in the range 0..255
  * @note We have a maximum of 3 PWM channels for this device - one is reserved for the display... the other two are reconfigured dynamically when they are required.
  */
void MicroBitPin::setAnalogValue(int value)
{
    //check if this pin has an analogue mode...
    if(!(PIN_CAPABILITY_ANALOG & capability))
        return;
        
    //sanitise the brightness level
    if(value < 0 || value > 255)
        return;
        
    float level = (float)value / float(MICROBIT_PIN_MAX_OUTPUT);
    
    // Move into an analogue input state if necessary.
    if (!(status & IO_STATUS_ANALOG_OUT)){
        disconnect();  
        pin = (void *)DynamicPwm::allocate(name);
        status |= IO_STATUS_ANALOG_OUT;
    }
    
    //perform a write!
    if(((DynamicPwm *)pin)->getPinName() == name)
        ((DynamicPwm *)pin)->write(level);
}


/**
  * Configures this IO pin as an analogue input (if necessary and possible).
  * @return the current analogue level on the pin, in the range 0-0xFFFF
  * 
  * Example:
  * @code 
  * MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_BOTH);
  * P0.getAnalogValue(); // P0 is a value in the range of 0 - 0xFFFF
  * @endcode
  */
int MicroBitPin::getAnalogValue()
{
    
    //check if this pin has an analogue mode...
    if(!(PIN_CAPABILITY_ANALOG & capability))
        return MICROBIT_IO_OP_NA;
        
    // Move into an analogue input state if necessary.
    if (!(status & IO_STATUS_ANALOG_IN)){
        disconnect();  
        pin = new AnalogIn(name);
        status |= IO_STATUS_ANALOG_IN;
    }
    
    //perform a read!
    return ((AnalogIn *)pin)->read_u16();
}

/**
  * Enables asynchronous callback events from this Pin.
  * When enabled, all state change updates will be propogated 
  * along the MicroBitMessageBus using the device's ID.
  * @note NOT YET IN USE 
  */     
void MicroBitPin::enableCallback()
{
}

/**
  * Disables asynchronous callback events from this Pin.
  * When disabled no state change updates will be propogated 
  * along the MicroBitMessageBus from this button.
  * @note NOT YET IN USE 
  */     
void MicroBitPin::disableCallback()
{
}
