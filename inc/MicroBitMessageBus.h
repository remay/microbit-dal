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
  
#ifndef MICROBIT_MESSAGE_BUS_H
#define MICROBIT_MESSAGE_BUS_H

#include "mbed.h"

// Enumeration of core components.
#define MICROBIT_CONTROL_BUS_ID         0

#define MICROBIT_BUS_ID_ANY				0
#define MICROBIT_BUS_VALUE_ANY			0



struct MicroBitEvent
{
	int source;         // ID of the MicroBit Component that generated the event � e.g. MICROBIT_ID_LEFT_BUTTON. 
    int value;          // Component specific code indicating the cause of the event.
    int timestamp;		// Time at which the event was generated. ms since power on?
    void *context;		// context specfic data associated with the event. 
};

struct MicroBitListener
{
	int id;								// The ID of the component that this listener is interested in. 
	int value;							// Value this listener is interested in receiving. 
	void (*cb)(MicroBitEvent *);		// Callback function associated with this listener.
	MicroBitListener *next;

	MicroBitListener(int id, int value, void (*messageBus)(MicroBitEvent *));
};

struct MicroBitMessageBusCache
{
	int seq;
	MicroBitListener *ptr;
};
	


class MicroBitMessageBus
{
    public:

	/**
	  * Constructor. 
	  * Anticipating only one MessageBus per device, as filtering is handled within the class.
	  */
    MicroBitMessageBus();    

	/**
	  * Send the given event to all regstered recipients.
	  *
	  * @param The event to send. This structure is assumed to be heap allocated, and will 
	  * be automatically freed once all recipients have been notified.
	  */
	void send(MicroBitEvent *evt);

	/**
	  * Send the given event to all regstered recipients, using a cached entry to minimize lookups.
	  * This is particularly useful for soptimizing ensors that frequently send to the same channel.
	  *
	  * @param evt The event to send. This structure is assumed to be heap allocated, and will 
	  * be automatically freed once all recipients have been notified.
	  * @param c Cache entry to reduce lookups for commonly used channels.
	  */
	void send(MicroBitEvent *evt, MicroBitMessageBusCache *c);

	/**
	  * Register a listener function.
	  * 
	  * @param id The source of messages to listen for. Events sent from any other IDs will be filtered. 
	  * Use MICROBIT_ID_ANY to receive events from all components.
	  *
	  * @param value The value of messages to listen for. Events with any other values will be filtered. 
	  * Use MICROBIT_VALUE_ANY to receive events of any value.
	  *
	  * @param hander The function to call when an event is received.
	  */

	void listen(int id, int value, void (*handler)(MicroBitEvent *));

	private:
	MicroBitListener *listeners;		// Chain of active listeners.
	int seq;							// Sequence number. Used to invalidate cache entries.
};

#endif


