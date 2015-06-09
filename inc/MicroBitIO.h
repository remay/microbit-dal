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
    MicroBitPin          P3;
    MicroBitPin          P4;
    MicroBitPin          P5;
    MicroBitPin          P6;
    MicroBitPin          P7;
    MicroBitPin          P8;
    MicroBitPin          P9;
    MicroBitPin          P10;
    MicroBitPin          P11;
    MicroBitPin          P12;
    MicroBitPin          P13;
    MicroBitPin          P14;
    MicroBitPin          P15;
    MicroBitPin          P16;
    MicroBitPin          P19;
    MicroBitPin          P20;
    
    /**
      * Constructor. 
      * Create a representation of all given I/O pins on the edge connector
      * @param id the event ID of the new MicroBitPin object.
      */
    MicroBitIO(int MICROBIT_ID_IO_P0, int MICROBIT_ID_IO_P1, int MICROBIT_ID_IO_P2,
               int MICROBIT_ID_IO_P3, int MICROBIT_ID_IO_P4, int MICROBIT_ID_IO_P5,
               int MICROBIT_ID_IO_P6, int MICROBIT_ID_IO_P7, int MICROBIT_ID_IO_P8,
               int MICROBIT_ID_IO_P9, int MICROBIT_ID_IO_P10,int MICROBIT_ID_IO_P11,
               int MICROBIT_ID_IO_P12,int MICROBIT_ID_IO_P13,int MICROBIT_ID_IO_P14,
               int MICROBIT_ID_IO_P15,int MICROBIT_ID_IO_P16,int MICROBIT_ID_IO_P19,
               int MICROBIT_ID_IO_P20);
    
    
};

#endif

