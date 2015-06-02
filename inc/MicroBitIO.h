/**
  * Class definition for MicroBit Button.
  *
  * Represents a single button on the device.
  */
  
#ifndef MICROBIT_IO_H
#define MICROBIT_IO_H

#include "mbed.h"
#include "MicroBitPin.h"
                                                     // Status Field flags...
#define IO_STATUS_DIGITAL_IN             0x01        // Pin is configured as an input, with a weak internal pull up.
#define IO_STATUS_DIGITAL_OUT            0x02        // Pin is configured as an output
#define IO_STATUS_ANALOG_IN              0x04        // Pin is Digital I/O (analog otherwise)    
#define IO_STATUS_ANALOG_OUT             0x08        // Pin is Analog I/O (analog otherwise)    
#define IO_STATUS_EVENTBUS_ENABLED       0x10        // Pin is will generate events on change


class MicroBitIO
{   
    public:
    
    MicroBitPin          P0;
    MicroBitPin          P1;
    MicroBitPin          P2;
    
    /**
      * Constructor. 
      * Create a Button representation with the given ID.
      * @param id the ID of the new LED object.
      */
    MicroBitIO(int MICROBIT_ID_IO_P0,int MICROBIT_ID_IO_P1, int MICROBIT_ID_IO_P2);
    
    
};

#endif

