/**
  * Class definition for a MicroBitEvent.
  *
  * The MicroBitEvent is the event object that represents an event that has occurred on the ubit.
  */

#include "MicroBit.h"

MicroBitEvent::MicroBitEvent(int source, int value, int timestamp, void *context, bool fire)
{
    this->source = source;
    this->value = value;
    this->timestamp = timestamp;
    this->context = context;   
    
    if(fire)
        this->fire();
}    

MicroBitEvent::MicroBitEvent()
{
    this->source = 0;
    this->value = 0;
    this->timestamp = 0;
    this->context = (void *)0;
}
//asdasd


//asdasdasd
void MicroBitEvent::fire()
{
    uBit.MessageBus.send(*this);
}