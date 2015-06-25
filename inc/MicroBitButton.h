#ifndef MICROBIT_BUTTON_H
#define MICROBIT_BUTTON_H

#include "mbed.h"

//TODO: When platform is built for MB2 - pins will be defined by default, these will change...
#define MICROBIT_PIN_BUTTON_A                   P0_17
#define MICROBIT_PIN_BUTTON_B                   P0_26
#define MICROBIT_PIN_BUTTON_RESET               P0_19

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


/**
  * Class definition for MicroBit Button.
  *
  * Represents a single, generic button on the device.
  */
class MicroBitButton
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */

    uint16_t id;                    // Event Bus ID
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
      *
      * Example:
      * @code 
      * buttonA(MICROBIT_ID_BUTTON_A,MICROBIT_PIN_BUTTON_A); //a number between 0 and 200 inclusive
      * @endcode
      *
      * Possible Events:
      * @code 
      * MICROBIT_BUTTON_EVT_DOWN
      * MICROBIT_BUTTON_EVT_UP
      * MICROBIT_BUTTON_EVT_CLICK
      * MICROBIT_BUTTON_EVT_LONG_CLICK
      * @endcode
      */
    MicroBitButton(uint16_t id, PinName name);
    
    /**
      * Tests if this Button is currently pressed.
      * @return 1 if this button is pressed, 0 otherwise.
      *
      * Example:
      * @code 
      * if(uBit.buttonA.isPressed())
      *     print("Pressed!");
      * @endcode
      */
    int isPressed();
    
    /**
      * periodic callback from MicroBit clock.
      * Check for state change for this button, and fires a hold event if button is pressed.
      */  
    void tick();
    
};

#endif
