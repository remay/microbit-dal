#include "inc/MicroBitPin.h"

/**
  * Constructor. 
  * Create an Pin representation with the given ID.
  * @param id the event ID of the new pin object.
  * @param name the #defined pin name contained in MicroBitPin.h
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
    // A shame, given they share some common method signatures... even delete! :-)
    if (status & IO_STATUS_DIGITAL_IN)
        delete ((DigitalIn *)pin);

    if (status & IO_STATUS_DIGITAL_OUT)
        delete ((DigitalOut *)pin);

    if (status & IO_STATUS_ANALOG_IN)
        delete ((AnalogIn *)pin);
    
    /*
    //REMOVED WHILE WE WORK OUT WHERE AnalogOut is...
    if (status & IO_STATUS_ANALOG_OUT)
        delete ((AnalogOut *)pin);*/
        
    this->pin = NULL;
    this->status = status & IO_STATUS_EVENTBUS_ENABLED;
}

/**
  * Configures this IO pin as a digital output (if necessary) and sets the pin to the given value.
  * @param value The new value for this digital output.
  */
void MicroBitPin::setDigitalValue(int value)
{
    //check if this pin has a digital mode...
    if(!(PIN_CAPABILITY_DIGITAL & capability))
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
  */
int MicroBitPin::getDigitalValue()
{
    //check if this pin has a digital mode...
    if(!(PIN_CAPABILITY_DIGITAL & capability))
        return 0;
    
    // Move into a Digital input state if necessary.
    if (!(status & IO_STATUS_DIGITAL_IN)){
        disconnect();  
        pin = new DigitalIn(name);
        status |= IO_STATUS_DIGITAL_IN;
    }
    
    return ((DigitalIn *)pin)->read();
}

/**
  * Configures this IO pin as an analogue output (if necessary and possible).
  * Change the DAC value to the given level.
  *
  * CURRENTLY NOT IN USE AS THERE IS NO DAC
  *
  * @param value the level to set on the output pin as
  */
void MicroBitPin::setAnalogValue(int value)
{
    /**
      * Could be doing something clever here with PWM in the future - for now, this is an empty function
      */
}


/**
  * Configures this IO pin as an analogue input (if necessary and possible).
  * @return the current analogue level on the pin as a float, in the range 0.0-1.0 
  */
int MicroBitPin::getAnalogValue()
{
    
    //check if this pin has an analogue mode...
    if(!(PIN_CAPABILITY_ANALOG & capability))
        return 0.0;
        
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
  * Enables asynchronous callback events from this button.
  * When enabled, all state change updates will be propogated 
  * along the MicroBitMessageBus using the device's ID.
  */    
void MicroBitPin::enableCallback()
{
}

 /**
  * Disables asynchronous callback events from this button.
  * When disabled no state change updates will be propogated 
  * along the MicroBitMessageBus from this button.
  */    
void MicroBitPin::disableCallback()
{
}
