/**
  * Class definition for MicroBit Button.
  *
  * Represents a single button on the device.
  */
  
#ifndef MICROBIT_IO_H
#define MICROBIT_IO_H

#include "mbed.h"
                                                     // Status Field flags...
#define IO_STATUS_DIGITAL_IN             0x01        // Pin is configured as an input, with a weak internal pull up.
#define IO_STATUS_DIGITAL_OUT            0x02        // Pin is configured as an output
#define IO_STATUS_ANALOG_IN              0x04        // Pin is Digital I/O (analog otherwise)    
#define IO_STATUS_ANALOG_OUT             0x08        // Pin is Analog I/O (analog otherwise)    
#define IO_STATUS_EVENTBUS_ENABLED       0x10        // Pin is will generate events on change


class MicroBitIO
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
      
    int id;             // Event Bus ID
    PinName name;       // mBed pin name of this pin.
    void *pin;          // The mBed object looking after this pin at any point in time (may change!).
    int status;         // latched state
   
    /**
      * Disconnect any attached mBed IO from this pin.
      * Used only when pin changes mode (i.e. Input/Output/Analog/Digital)
      */
    void disconnect();
    
    public:
    /**
      * Constructor. 
      * Create a Button representation with the given ID.
      * @param id the ID of the new LED object.
      */
    MicroBitIO(int id, PinName name);
    
    /**
      * Configures this IO pin as a digital output (if necessary) and sets the pin HI.
      */
    void setDigitalValue(int value);

    /**
      * Configures this IO pin as a digital output (if necessary) and sets the pin LO.
      */
    void clearDigital();
    
    /**
      * Configures this IO pin as a digital input (if necessary) and tests its current value.
      * @return 1 if this input is high, 0 otherwise.
      */
    int getDigitalValue();

    /**
      * Configures this IO pin as an analogue output (if necessary and possible).
      * Change the DAC value to the given level.
      * @param value the level to set on the output pin, in the range 0..???
      */
    void setAnalogValue(int value);


    /**
      * Configures this IO pin as an analogue input (if necessary and possible).
      * @return the current analogue level on the pin, in the range 0..???
      */
    int getAnalogValue();

     /**
      * Enables asynchronous callback events from this button.
      * When enabled, all state change updates will be propogated 
      * along the MicroBitMessageBus using the device's ID.
      */    
    void enableCallback();
    
     /**
      * Disables asynchronous callback events from this button.
      * When disabled no state change updates will be propogated 
      * along the MicroBitMessageBus from this button.
      */    
    void disableCallback();
    
};

#endif

