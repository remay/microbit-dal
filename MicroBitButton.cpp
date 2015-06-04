#include "inc/MicroBit.h"
#include "inc/MicroBitButton.h"
#include "inc/MicroBitMessageBus.h"

/**
  * Constructor. 
  * Create a button representation with the given ID.
  * @param id the ID of the new LED object.
  * @param name the physical pin on the processor that this butotn is connected to.
  */
MicroBitButton::MicroBitButton(int id, PinName name) : pin(name), irq(name)
{
    this->id = id;
    this->name = name;
    this->eventStartTime = 0;
    this->downStartTime = 0;
    
    irq.rise(this, &MicroBitButton::risingFalling);
    irq.fall(this, &MicroBitButton::risingFalling);
}

void MicroBitButton::risingFalling()
{   
    eventStartTime = ticks;
}
#ifdef NOB
/**
  * Interrupt on change handler for this button.
  * Button up
  */
void MicroBitButton::rising()
{   
    //if the debounce up flag is already set, we ignore this button release
    if(!(status & MICROBIT_BUTTON_DEBOUNCE_UP))
    {
        //else begin the debounceUpTimeout and set the status
        debounceUpTimeout.attach(this, &MicroBitButton::debounceUp, MICROBIT_BUTTON_DEBOUNCE_PERIOD);
        status |= MICROBIT_BUTTON_DEBOUNCE_UP;       
    }
}


/**
  * Interrupt on change handler for this button.
  * Button down
  */
void MicroBitButton::falling()
{
    //if the debounce down flag is already set, we ignore this button press
    if(!(status & MICROBIT_BUTTON_DEBOUNCE_DOWN))
    {
        //else begin the debounceDownTimeout and set the status
        debounceDownTimeout.attach(this, &MicroBitButton::debounceDown, MICROBIT_BUTTON_DEBOUNCE_PERIOD);
        status |= MICROBIT_BUTTON_DEBOUNCE_DOWN;       
    }
}

#endif

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
  * Debounce handler called by debounceDownTimeout, triggered by falling() interrupt
  
void MicroBitButton::debounceDown()
{
    if(eventStartTime == 0)
    {
        //send a button down event
        MicroBitEvent evt;
        evt.source = id;
        evt.context = NULL;
        evt.timestamp = ticks;
        evt.value = MICROBIT_BUTTON_EVT_DOWN;
        uBit.MessageBus.send(evt);
        
        //set the eventStartTime,
        eventStartTime=ticks;        
    } 
    
    //if still pressed, call me back!
    if(isPressed() && (status & MICROBIT_BUTTON_DEBOUNCE_DOWN))
        debounceDownTimeout.attach(this, &MicroBitButton::debounceDown, MICROBIT_BUTTON_DEBOUNCE_PERIOD);   
}
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
  * debounceUp() handler called by debounceUpTimeout, triggered by rising() interrupt
  * The logic behind having a debounce up method is to protect against an accidental release
  
void MicroBitButton::debounceUp()
{
    // check if the button is still no longer pressed, if it is fire events, otherwise, call me back!
    if(!isPressed())
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
        if((ticks-eventStartTime)>=MICROBIT_BUTTON_DEBOUNCE_LONG)    
            evt.value = MICROBIT_BUTTON_EVT_LONG_CLICK;
        else
            evt.value = MICROBIT_BUTTON_EVT_CLICK;
            
        //send long click or normal click events
        uBit.MessageBus.send(evt);
        
        //reset status and eventStartTime.
        status = 0x00;
        eventStartTime = 0; 
    }
    else
        debounceUpTimeout.attach(this, &MicroBitButton::debounceUp, MICROBIT_BUTTON_DEBOUNCE_PERIOD); // button is pressed again 
}*/


void MicroBitButton::tick(){
    if(eventStartTime == 0)
        return;
        
    if(ticks > eventStartTime + MICROBIT_BUTTON_DEBOUNCE_PERIOD){
        eventStartTime = 0;
        status = !pin;
 
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

