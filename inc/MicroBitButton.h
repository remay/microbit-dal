/**
  * Class definition for MicroBit Button.
  *
  * Represents a single button on the device.
  */
  
#ifndef MICROBIT_BUTTON_H
#define MICROBIT_BUTTON_H

#include "mbed.h"

//TODO: When platform is built for MB2 - pins will be defined by default, these will change...
#define MICROBIT_PIN_BUTTON_A                   P0_17
#define MICROBIT_PIN_BUTTON_B                   P0_26

#define MICROBIT_BUTTON_EVT_DOWN                1
#define MICROBIT_BUTTON_EVT_UP                  2
#define MICROBIT_BUTTON_EVT_CLICK               3
#define MICROBIT_BUTTON_EVT_LONG_CLICK          4
#define MICROBIT_BUTTON_EVT_HOLD                5

#define MICROBIT_BUTTON_DEBOUNCE_PERIOD         0.02  //the time to set debounceDownTimeout and debounceUpTimeout to.
#define MICROBIT_BUTTON_DEBOUNCE_LONG           1000  //1000 ticks is a LONG_CLICK
#define MICROBIT_BUTTON_DEBOUNCE_HOLD           1500  //1500 ticks is a hold

#define MICROBIT_BUTTON_STATE                   1
#define MICROBIT_BUTTON_STATE_SET               2
#define MICROBIT_BUTTON_STATE_HOLD_TRIGGERED    4



class MicroBitButton
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */

    int id;                         // Event Bus ID
    PinName name;                   // mBed pin name of this pin.
    DigitalIn pin;                  // The mBed object looking after this pin at any point in time (may change!).
    uint8_t status;                 // Keeps track of the button state when performing debounce detection.
    
    unsigned long eventStartTime;   // used to store the current system clock when button state changes
    unsigned long downStartTime;    // used to store the current system clock when a button down event occurs
    
    void debounceDown();            // Handles when a the state of the button has been changed to pressed, after a debounce.
    void debounceUp();              // Handles when a the state of the button has been changed to released, after a debounce.
    
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
      * Check for state change for this button, and fires a hold event if button is pressed.
      */  
    void tick();
    
};

#endif
