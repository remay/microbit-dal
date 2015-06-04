#include "inc/MicroBit.h"
#include "inc/MicroBitButton.h"
#include "inc/MicroBitMessageBus.h"

/**
  * Constructor. 
  * Create a button representation with the given ID.
  * @param id the ID of the new LED object.
  * @param name the physical pin on the processor that this butotn is connected to.
  */
MicroBitButton::MicroBitButton(int id, PinName name) : pin(name)
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
    MicroBitEvent evt;
    evt.source = id;
    evt.context = NULL;
    evt.timestamp = ticks;
    evt.value = MICROBIT_BUTTON_EVT_DOWN;
    uBit.MessageBus.send(evt);
    
    //set the downStartTime,
    downStartTime=ticks;        
}

/**
  * Handles when a the state of the button has been changed to released, after a debounce.
  */
void MicroBitButton::debounceUp()
{
    //create an Event object
    MicroBitEvent evt;

    //send button up event
    evt.source = id;
    evt.context = NULL;
    evt.timestamp = ticks;
    evt.value = MICROBIT_BUTTON_EVT_UP;
    uBit.MessageBus.send(evt);
    
    //determine if this is a long click or a normal click and update the previous used evt value
    if((ticks-downStartTime)>=MICROBIT_BUTTON_DEBOUNCE_LONG)    
        evt.value = MICROBIT_BUTTON_EVT_LONG_CLICK;
    else
        evt.value = MICROBIT_BUTTON_EVT_CLICK;
        
    //send long click or normal click events
    uBit.MessageBus.send(evt);
}

/**
  * periodic callback from MicroBit clock.
  * Check for state change for this button, and fires a hold event if button is pressed.
  */  
void MicroBitButton::tick()
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
        
        //create an Event object
        MicroBitEvent evt;
    
        //send button up event
        evt.source = id;
        evt.context = NULL;
        evt.timestamp = ticks;
        evt.value = MICROBIT_BUTTON_EVT_HOLD;
        uBit.MessageBus.send(evt);
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

