#include "inc/MicroBitIO.h"

/**
  * Constructor. 
  * Create an LED representation with the given ID.
  * @param id the ID of the new LED object.
  */
MicroBitIO::MicroBitIO(int id, PinName name) 
{   
    this->id = id;
    this->name = name;
    
    // Power up in a disconnected, low power state.
    // If we're unused, this is how it will stay...
    this->status = 0x00;
    this->pin = NULL;
}

/**
  * Disconnect any attached mBed IO from this pin.
  * Used only when pin changes mode (i.e. Input/Output/Analog/Digital)
  */
void MicroBitIO::disconnect()
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
        
    this->pin = NULL;
    this->status = status & IO_STATUS_EVENTBUS_ENABLED;
}

/**
  * Configures this IO pin as a digital output (if necessary) and sets the pin to the given value.
  * @param value The new value for this digital output.
  */
void MicroBitIO::setDigitalValue(int value)
{
    // Move into a Digital input state if necessary.
    if (!(status & IO_STATUS_DIGITAL_OUT)){
        disconnect();  
        pin = new DigitalOut(name);
        status |= IO_STATUS_DIGITAL_OUT;
    }
    
    ((DigitalOut *)pin)->write(value);
}

/**
  * Configures this IO pin as a digital input (if necessary) and tests its current value.
  * @return 1 if this input is high, 0 otherwise.
  */
int MicroBitIO::getDigitalValue()
{
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
  * @param value the level to set on the output pin, in the range 0..???
  */
void MicroBitIO::setAnalogValue(int value)
{
    // This platform doesn't actually have a DAC. :-)
}


/**
  * Configures this IO pin as an analogue input (if necessary and possible).
  * @return the current analogue level on the pin, in the range 0..???
  */
int MicroBitIO::getAnalogValue()
{
    return 0;
}

 /**
  * Enables asynchronous callback events from this button.
  * When enabled, all state change updates will be propogated 
  * along the MicroBitMessageBus using the device's ID.
  */    
void MicroBitIO::enableCallback()
{
}

 /**
  * Disables asynchronous callback events from this button.
  * When disabled no state change updates will be propogated 
  * along the MicroBitMessageBus from this button.
  */    
void MicroBitIO::disableCallback()
{
}
