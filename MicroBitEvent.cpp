/**
  * Class definition for a MicroBitEvent.
  *
  * The MicroBitEvent is the event object that represents an event that has occurred on the ubit.
  */

#include "MicroBit.h"

/**
  * Constructor. 
  * @param src ID of the MicroBit Component that generated the event e.g. MICROBIT_ID_BUTTON_A.
  * @param value Component specific code indicating the cause of the event.
  * @param timestamp Time at which the event was generated. ms since power on.
  * @param context context specfic data associated with the event.
  * @param fire whether the event should be fire immediately upon construction
  */
MicroBitEvent::MicroBitEvent(int source, int value, int timestamp, void *context, bool fire)
{
    this->source = source;
    this->value = value;
    this->timestamp = timestamp;
    this->context = context;   
    
    if(fire)
        this->fire();
}    

//empty constructor.
MicroBitEvent::MicroBitEvent()
{
    this->source = 0;
    this->value = 0;
    this->timestamp = 0;
    this->context = (void *)0;
}

/**
  * Fires the represented event onto the message bus!
  */
void MicroBitEvent::fire()
{
    uBit.MessageBus.send(*this);
}