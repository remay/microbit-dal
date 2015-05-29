/**
  * Class definition for a MicroBit device.
  *
  * Represents the device as a whole, and includes member variables to that reflect the components of the system.
  */
  
#ifndef MICROBIT_H
#define MICROBIT_H

// DEBUG. Enable this to get debug message routed through the USB serial interface.
#define MICROBIT_DBG

//#define MICROBIT_BLE

#include "mbed.h"
#include "BLEDevice.h"
#include "DeviceInformationService.h"

#include "MicroBitCompat.h"
#include "MicroBitFiber.h"
#include "ManagedString.h"
#include "MicroBitImage.h"
#include "MicroBitMessageBus.h"

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

// Random number generator
#define NRF51822_RNG_ADDRESS            0x4000D000

#define MICROBIT_IO_PINS                8            // TODO: Need to know how many. :-)

// Enumeration of core components.
#define MICROBIT_ID_LEFT_BUTTON         1
#define MICROBIT_ID_RIGHT_BUTTON        2
#define MICROBIT_ID_ACCELEROMETER       3
#define MICROBIT_ID_MAGNETOMETER        4
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
#define MICROBIT_PIN_SDA                P0_30
#define MICROBIT_PIN_SCL                P0_0

extern Serial pc;

class MicroBit
{                                    
    public:
    
    // Map of device state.
    uint32_t                flags;

    // I2C Interface
    I2C                     i2c;   

    // Device level Message Bus abstraction
    MicroBitMessageBus      MessageBus;      
    
    // Member variables to represent each of the core components on the device.
    MicroBitDisplay         display;
    MicroBitButton          leftButton;
    MicroBitButton          rightButton;
    MicroBitAccelerometer   accelerometer;
    
/*


    MicroBitIO          pins[MICROBIT_IO_PINS];
*/    
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
      * Delay for the givne amount of time.
      * If the scheduler is running, this will deschedule the current fiber and perform
      * a power efficent, concurrent sleep operation.
      * If the scheduler is disabled or we're running in an interrupt context, this
      * will revert to a busy wait. 
      */
    void sleep(int milliseconds);

    int random(int max);

};

// Definition of the global instance of the MicroBit class.
// Using this as a variation on the singleton pattern, just to make
// code integration a littl bit easier for 3rd parties.
extern MicroBit uBit;

// Entry point for application programs. Called after the super-main function
// has initialized the device and runtime environment.
extern "C" void app_main();

// FOR TESTING ONLY...
extern "C" void updateScroll(char *str, int len);



#endif

