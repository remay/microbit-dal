/**
  * Class definition for a MicroBit device.
  *
  * Represents the device as a whole, and includes member variables to that reflect the components of the system.
  */
  
#ifndef MICROBIT_H
#define MICROBIT_H

// DEBUG. Enable this to get debug message routed through the USB serial interface.
//#define MICROBIT_DBG

//#define MICROBIT_BLE


#include "mbed.h"
#include "BLEDevice.h"
#include "DeviceInformationService.h"

//error number enumeration
#include "ErrorNo.h"

#include "MicroBitCompat.h"
#include "MicroBitFiber.h"
#include "ManagedString.h"
#include "MicroBitImage.h"
#include "MicroBitEvent.h"
#include "MicroBitMessageBus.h"
#include "SmartPwm.h"

#include "MicroBitButton.h"
#include "MicroBitDisplay.h"
#include "MicroBitIO.h"
#include "MicroBitLED.h"
#include "MicroBitCompass.h"
#include "MicroBitAccelerometer.h"

#include "MicroBitDFUService.h"

// MicroBit::flags values
#define MICROBIT_FLAG_SCHEDULER_RUNNING         0x00000001
#define MICROBIT_FLAG_ACCELEROMETER_RUNNING     0x00000002
#define MICROBIT_FLAG_DISPLAY_RUNNING           0x00000004
#define MICROBIT_FLAG_COMPASS_RUNNING           0x00000008

// Random number generator
#define NRF51822_RNG_ADDRESS            0x4000D000

#define MICROBIT_IO_PINS                3            // TODO: Need to change for live, currently 3 for test

// Enumeration of core components.
#define MICROBIT_ID_BUTTON_A            1
#define MICROBIT_ID_BUTTON_B            2
#define MICROBIT_ID_ACCELEROMETER       3
#define MICROBIT_ID_COMPASS             4
#define MICROBIT_ID_DISPLAY             5

//EDGE connector events
#define MICROBIT_ID_IO_P0               6           //P0 is the left most pad (ANALOG/DIGITAL) 
#define MICROBIT_ID_IO_P1               7           //P1 is the middle pad (ANALOG/DIGITAL) 
#define MICROBIT_ID_IO_P2               8           //P2 is the right most pad (ANALOG/DIGITAL) 
#define MICROBIT_ID_IO_P3               9           //COL1 (ANALOG/DIGITAL) 
#define MICROBIT_ID_IO_P4               10          //BTN_A        
#define MICROBIT_ID_IO_P5               11          //COL2 (ANALOG/DIGITAL) 
#define MICROBIT_ID_IO_P6               12          //ROW2
#define MICROBIT_ID_IO_P7               13          //ROW1       
#define MICROBIT_ID_IO_P8               14          //PIN 18
#define MICROBIT_ID_IO_P9               15          //ROW3                  
#define MICROBIT_ID_IO_P10              16          //COL3 (ANALOG/DIGITAL) 
#define MICROBIT_ID_IO_P11              17          //BTN_B
#define MICROBIT_ID_IO_P12              18          //PIN 20
#define MICROBIT_ID_IO_P13              19          //SCK
#define MICROBIT_ID_IO_P14              20          //MISO
#define MICROBIT_ID_IO_P15              21          //MOSI
#define MICROBIT_ID_IO_P16              22          //PIN 16
#define MICROBIT_ID_IO_P19              23          //SCL
#define MICROBIT_ID_IO_P20              24          //SDA


// mBed pin assignments of core components.
//TODO: When platform is built for MB2 - pins will be defined by default, these will change...
#define MICROBIT_PIN_SDA                P0_30
#define MICROBIT_PIN_SCL                P0_0

extern Serial pc;

class MicroBit
{                                    
    public:
    
    // Map of device state.
    uint32_t                flags;

    // Periodic callback
    Ticker                  systemTicker;

    // I2C Interface
    I2C                     i2c;   


    // Device level Message Bus abstraction
    MicroBitMessageBus      MessageBus;      
    
    // Member variables to represent each of the core components on the device.
    MicroBitDisplay         display;
    MicroBitButton          buttonA;
    MicroBitButton          buttonB;
    MicroBitAccelerometer   accelerometer;
    MicroBitCompass         compass;

    //An array of available IO pins on the device
    MicroBitIO              io;
    
    // Bluetooth related member variables.
    BLEDevice                   *ble;
    DeviceInformationService    *ble_device_information_service;
    MicroBitDFUService          *ble_firmware_update_service;

    
    /**
      * Constructor. 
      * Create a representation of a MicroBit device.
      * @param messageBus callback function to receive MicroBitMessageBus events.
      */
    MicroBit();  

    /**
      * Post constructor initialisation method.
      * After *MUCH* pain, it's noted that the BLE stack can't be brought up in a 
      * static context, so we bring it up here rather than in the constructor.
      * n.b. This method *must* be called in main() or later, not before.
      */
    void init();

    /**
      * Delay for the given amount of time.
      * If the scheduler is running, this will deschedule the current fiber and perform
      * a power efficent, concurrent sleep operation.
      * If the scheduler is disabled or we're running in an interrupt context, this
      * will revert to a busy wait. 
      */
    void sleep(int milliseconds);

    /**
      * Generate a randoim number in the given range.
      * We use the NRF51822 in built random number generator here
      * TODO: Determine if we want to, given its relatively high power consumption!
      *
      * @return A random, natural number between 0 and the the given maximum value.
      */
    int random(int max);

    /**
      * Period callback. Used by MicroBitDisplay, FiberScheduler and I2C sensors to
      * provide a power efficient sense of time.
      *
      */
    void systemTick();

    /**
      * Determine the time since this MicroBit was last reset.
      *
      * @return The time since the last reset, in milliseconds.
      */
    unsigned long systemTime();
    
    /**
      * Triggers a microbit panic where an infinite loop will occur swapping between the panicFace and statusCode if provided.
      * TODO: refactor this so that it doesn't rely on instantiating new variables as memory will not be available.
      * @param statusCode the status code of the associated error - TBD
      */
    void panic(int statusCode = 0);

};

// Definition of the global instance of the MicroBit class.
// Using this as a variation on the singleton pattern, just to make
// code integration a littl bit easier for 3rd parties.
extern MicroBit uBit;

// Entry point for application programs. Called after the super-main function
// has initialized the device and runtime environment.
extern "C" void app_main();


#endif

