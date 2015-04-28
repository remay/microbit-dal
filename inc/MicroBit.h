/**
  * Class definition for a MicroBit device.
  *
  * Represents the device as a whole, and includes member variables to that reflect the components of the system.
  */
  
#ifndef MICROBIT_H
#define MICROBIT_H

#include "mbed.h"
#include "MicroBitMessageBus.h"
#include "MicroBitButton.h"
#include "MicroBitDisplay.h"
#include "MicroBitImage.h"
#include "MicroBitIO.h"
#include "MicroBitLED.h"
#include "MicroBitFiber.h"

#define MICROBIT_IO_PINS       8            // TODO: Need to know how many. :-)

// Enumeration of core components.
#define MICROBIT_ID_LEFT_BUTTON         1
#define MICROBIT_ID_RIGHT_BUTTON        2
#define MICROBIT_ID_USER_LED            3
#define MICROBIT_ID_DISPLAY             5
#define MICROBIT_ID_IO_1                6
#define MICROBIT_ID_IO_2                7
#define MICROBIT_ID_IO_3                8
#define MICROBIT_ID_IO_4                9
#define MICROBIT_ID_IO_5                10
#define MICROBIT_ID_IO_6                11
#define MICROBIT_ID_IO_7                12
#define MICROBIT_ID_IO_8                13

// mBed pin assignments of core components.
#define MICROBIT_PIN_USER_LED          P0_18


class MicroBit
{
    // Internal constructor. The ARM compiler doesn't seem to support constructor chaining...
    void init();                                    
    
    public:
    static MicroBitMessageBus MessageBus;      
    //
    // Add member variables to represent each of the core components on the device.
    //

    MicroBitLED         userLED;
    MicroBitDisplay     display;
    MicroBitButton      leftButton;
/*
    MicroBitButton      rightButton;

    MicroBitIO          pins[MICROBIT_IO_PINS];
*/    

    /**
      * Constructor. 
      * Create a representation of a MicroBit device.
      * @param messageBus callback function to receive MicroBitMessageBus events.
      */
    MicroBit();    
};

#endif

