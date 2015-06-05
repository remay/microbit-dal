/**
  * Class definition for the MicroBitMessageBus.
  *
  * The MicroBitMessageBus is the common mechanism to deliver asynchronous events on the
  * MicroBit platform. It serves a number of purposes:
  *
  * 1) It provides an eventing abstraction that is independent of the underlying substrate.
  * 2) It provides a mechanism to decouple user code from trusted system code 
  *    i.e. the basis of a message passing nano kernel.
  * 3) It allows a common high level eventing abstraction across a range of hardware types.e.g. buttons, BLE...
  * 4) It provides a mechanims for extensibility - new devices added via I/O pins can have OO based
       drivers and communicate via the message bus with minima impact on user level languages.
  * 5) It allows for the possiblility of event / data aggregation, which in turn can save energy.
  * It has the following design principles:
  *
  * 1) Maintain a low RAM footprint where possible
  * 2) Make few assumptions about the underlying platform, but allow optimizations where possible.
  */
  
#ifndef MICROBIT_EVENT
#define MICROBIT_EVENT

#include "mbed.h"

class MicroBitEvent
{
    public:

    //These are public at the moment for backwards compatability with old code
    //will be refactored in the future!
    
    int source;         // ID of the MicroBit Component that generated the event e.g. MICROBIT_ID_BUTTON_A. 
    int value;          // Component specific code indicating the cause of the event.
    int timestamp;      // Time at which the event was generated. ms since power on.
    void *context;      // context specfic data associated with the event. 

    /**
      * Constructor. 
      * @param src ID of the MicroBit Component that generated the event e.g. MICROBIT_ID_BUTTON_A.
      * @param value Component specific code indicating the cause of the event.
      * @param timestamp Time at which the event was generated. ms since power on.
      * @param context context specfic data associated with the event.
      */
    
    MicroBitEvent(int source, int value, int timestamp, void *context, bool fire=false);   
    MicroBitEvent(); 

    void fire();
};

#endif

