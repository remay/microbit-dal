/**
  * Class definition for a MicroBit device.
  *
  * Represents the device as a whole, and includes member variables to that reflect the components of the system.
  */
  
#ifndef MICROBIT_H
#define MICROBIT_H

#include "mbed.h"
#include "MicroBitCompat.h"
#include "MicroBitMessageBus.h"
#include "MicroBitButton.h"
#include "MicroBitDisplay.h"
#include "MicroBitImage.h"
#include "MicroBitIO.h"
#include "MicroBitLED.h"
#include "MicroBitFiber.h"
#include "BLEDevice.h"
#include "DeviceInformationService.h"
#include "DFUService.h"


#define MICROBIT_IO_PINS                8            // TODO: Need to know how many. :-)

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
#define MICROBIT_PIN_USER_LED           P0_18
#define MICROBIT_PIN_SDA                P0_22
#define MICROBIT_PIN_SCL                P0_20


class MicroBit
{                                    
    public:
    
    // Device level Message Bus abstraction
    MicroBitMessageBus  MessageBus;      

    // Member variables to represent each of the core components on the device.
    //MicroBitLED         userLED;
    MicroBitDisplay     *display;
    //MicroBitButton      leftButton;
    //I2C                 i2c;
/*
    MicroBitButton      rightButton;

    MicroBitIO          pins[MICROBIT_IO_PINS];
*/    
    // Bluetooth related member variables.
    BLEDevice                   *ble;
    DeviceInformationService    *ble_device_information_service;
    DFUService                  *ble_firmware_update_service;
    
    /**
      * Constructor. 
      * Create a representation of a MicroBit device.
      * @param messageBus callback function to receive MicroBitMessageBus events.
      */
    MicroBit();  
    
    void initDisplay();
    void initBLE();  
};

// Definition of the global instance of the MicroBit class.
// Using this as a variation on the singleton pattern, just to make
// code integration a littl bit easier for 3rd parties.
extern MicroBit uBit;


#endif

