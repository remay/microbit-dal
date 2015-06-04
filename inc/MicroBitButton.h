/**
  * Class definition for MicroBit Button.
  *
  * Represents a single button on the device.
  */
  
#ifndef MICROBIT_BUTTON_H
#define MICROBIT_BUTTON_H

#include "mbed.h"

//TODO: When platform is built for MB2 - pins will be defined by default, these will change...
#define MICROBIT_PIN_BUTTON_A             P0_17
#define MICROBIT_PIN_BUTTON_B             P0_26

#define MICROBIT_BUTTON_EVT_DOWN          1
#define MICROBIT_BUTTON_EVT_UP            2
#define MICROBIT_BUTTON_EVT_CLICK         3
#define MICROBIT_BUTTON_EVT_LONG_CLICK    4
#define MICROBIT_BUTTON_EVT_HOLD          5

#define MICROBIT_BUTTON_DEBOUNCE_PERIOD   0.02  //the time to set debounceDownTimeout and debounceUpTimeout to.
#define MICROBIT_BUTTON_DEBOUNCE_LONG     1000  //1000 ticks is a LONG_CLICK
#define MICROBIT_BUTTON_DEBOUNCE_HOLD     1500  //1500 ticks is a hold


class MicroBitButton
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
      
    int id;                         // Event Bus ID
    PinName name;                   // mBed pin name of this pin.
    DigitalIn pin;                  // The mBed object looking after this pin at any point in time (may change!).
    InterruptIn irq;                // Handler to detect change events.
    int status;                     // Keeps track of the button status when performing debounce detection.
       
    void risingFalling();           // Interrupt on change handler
    
    unsigned long eventStartTime;   // used to store the current system clock when a button down event occurs
    unsigned long downStartTime;    // used to store the current system clock when a button down event occurs
    
    void debounceDown();            // Debounce handler called by debounceDownTimeout, triggered by falling() interrupt
    void debounceUp();              // Debounce handler called by debounceUpTimeout, triggered by rising() interrupt
    
    public:
    /**
      * Constructor. 
      * Create a pin representation with the given ID.
      * @param id the ID of the new LED object.
      * @param name the physical pin on the processor that this butotn is connected to.
      */
    MicroBitButton(int id, PinName name);
    
    /**
      * Tests if this Button is currently pressed.
      * @return 1 if this button is pressed, 0 otherwise.
      */
    int isPressed();
    
    /**
      * periodic callback from MicroBit clock.
      * Check for state change for this button.
      */  
    void tick();
    
};

#endif
