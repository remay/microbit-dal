/**
  * Class definition for MicroBit Button.
  *
  * Represents a single button on the device.
  */
  
#ifndef MICROBIT_PIN_H
#define MICROBIT_PIN_H

#include "mbed.h"
                                                     // Status Field flags...
#define IO_STATUS_DIGITAL_IN             0x01        // Pin is configured as a digital input, with no pull up.
#define IO_STATUS_DIGITAL_OUT            0x02        // Pin is configured as a digital output
#define IO_STATUS_ANALOG_IN              0x04        // Pin is Analog in 
#define IO_STATUS_ANALOG_OUT             0x08        // Pin is Analog out (not currently possible)  
#define IO_STATUS_EVENTBUS_ENABLED       0x10        // Pin is will generate events on change

//#defines for each edge connector pin
#define MICROBIT_PIN_P0                  P0_3        //P0 is the left most pad (ANALOG/DIGITAL)  
#define MICROBIT_PIN_P1                  P0_2        //P1 is the middle pad (ANALOG/DIGITAL) 
#define MICROBIT_PIN_P2                  P0_1        //P2 is the right most pad (ANALOG/DIGITAL)  
#define MICROBIT_PIN_P3                  P0_4        //COL1 (ANALOG/DIGITAL) 
#define MICROBIT_PIN_P4                  P0_17       //BTN_A           
#define MICROBIT_PIN_P5                  P0_5        //COL2 (ANALOG/DIGITAL) 
#define MICROBIT_PIN_P6                  P0_14       //ROW2
#define MICROBIT_PIN_P7                  P0_13       //ROW1       
#define MICROBIT_PIN_P8                  P0_18       //PIN 18
#define MICROBIT_PIN_P9                  P0_15       //ROW3                  
#define MICROBIT_PIN_P10                 P0_6        //COL3 (ANALOG/DIGITAL) 
#define MICROBIT_PIN_P11                 P0_26       //BTN_B
#define MICROBIT_PIN_P12                 P0_20       //PIN 20
#define MICROBIT_PIN_P13                 P0_23       //SCK
#define MICROBIT_PIN_P14                 P0_22       //MISO
#define MICROBIT_PIN_P15                 P0_21       //MOSI
#define MICROBIT_PIN_P16                 P0_16       //PIN 16
#define MICROBIT_PIN_P17                 P0_0        //SCL
#define MICROBIT_PIN_P18                 P0_30       //SDA

/**
  * Pin capabilities enum. 
  * Used to determine the capabilities of each Pin as some can only be digital, or can be both digital and analogue.
  */
enum PinCapability{
    PIN_CAPABILITY_DIGITAL = 0x01,
    PIN_CAPABILITY_ANALOG = 0x02,
    PIN_CAPABILITY_BOTH = PIN_CAPABILITY_DIGITAL | PIN_CAPABILITY_ANALOG
};

class MicroBitPin
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
      
    int id;             // Event Bus ID
    PinName name;       // mBed pin name of this pin.
    void *pin;          // The mBed object looking after this pin at any point in time (may change!).
    int status;         // latched state
    PinCapability capability;
   
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
    MicroBitPin(int id, PinName name, PinCapability capability);
    
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
