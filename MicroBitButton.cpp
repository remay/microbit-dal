#include "inc/MicroBit.h"
#include "inc/MicroBitButton.h"
#include "inc/MicroBitMessageBus.h"

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
  * MICROBIT_BUTTON_EVT_HOLD
  * @endcode
  */
MicroBitButton::MicroBitButton(uint16_t id, PinName name) : pin(name)
{
    this->id = id;
    this->name = name;
    this->eventStartTime = 0;
    this->downStartTime = 0;
}

/**
  * Handles when a the state of the button has been changed to pressed, after a debounce.
  */
void MicroBitButton::debounceDown()
{
    //send a button down event
    MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_DOWN);
    
    //set the downStartTime,
    downStartTime=ticks;        
}

/**
  * Handles when a the state of the button has been changed to released, after a debounce.
  */
void MicroBitButton::debounceUp()
{
    //send button up event
    MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_UP);
    
    //determine if this is a long click or a normal click and send event
    if((ticks-downStartTime)>=MICROBIT_BUTTON_DEBOUNCE_LONG)
        MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_LONG_CLICK);    
    else
        MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_CLICK);
}

/**
  * periodic callback from MicroBit clock.
  * Check for state change for this button, and fires a hold event if button is pressed.
  */  
void MicroBitButton::systemTick()
{

    //if the status button state is different from the pin state, and we haven't set the button state before...
    if((status & MICROBIT_BUTTON_STATE) == pin && !(status & MICROBIT_BUTTON_STATE_SET))
    {
        status |= MICROBIT_BUTTON_STATE_SET;
        eventStartTime = ticks;    
    }

    //if button is pressed and the hold triggered event state is not triggered AND we are greater than the button debounce value
    if((status & MICROBIT_BUTTON_STATE) && !(status & MICROBIT_BUTTON_STATE_HOLD_TRIGGERED) && (ticks - downStartTime) >= MICROBIT_BUTTON_DEBOUNCE_HOLD)
    {
        //set the hold triggered event flag
        status |= MICROBIT_BUTTON_STATE_HOLD_TRIGGERED;
        
        //fire hold event
        MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_HOLD);
    }
    
    //handle button debounce, this ensure we don't get multiple button events for a single press
    //due to hardware.
    if(eventStartTime && ticks > eventStartTime + MICROBIT_BUTTON_DEBOUNCE_PERIOD)
    {
        //reset
        eventStartTime = 0;
        
        //set the new status and reset the STATE_SET flag
        status = !pin;
        
        //if pressed...
        if(status)
            debounceDown(); 
        else 
            debounceUp();
    }
}

/**
  * Tests if this Button is currently pressed.
  * @return 1 if this button is pressed, 0 otherwise.
  */
int MicroBitButton::isPressed()
{
    return status;
}

