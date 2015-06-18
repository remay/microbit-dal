/**
  * Class definition for a MicroBitMessageBus.
  *
  * The MicroBitMessageBus handles all messages passed between components.
  */

#include "MicroBit.h"

/**
  * Constructor. 
  * Create a new Message Bus Listener.
  * @param id The ID of the component you want to listen to.
  * @param value The event ID you would like to listen to from that component
  * @param handler A function pointer to call when the event is detected.
  */
MicroBitListener::MicroBitListener(int id, int value, void (*handler)(void))
{
	this->id = id;
	this->value = value;
	this->cb = handler;
	this->next = NULL;
}

/**
  * Constructor. 
  * Create a new Message Bus.
  */
MicroBitMessageBus::MicroBitMessageBus()
{
	this->listeners = NULL;
	this->seq = 0;
}

/**
  * Send the given event to all regstered recipients.
  *
  * @param The event to send. This structure is assumed to be heap allocated, and will 
  * be automatically freed once all recipients have been notified.
  */
void MicroBitMessageBus::send(MicroBitEvent &evt)
{
	this->send(evt, NULL);
}

/**
  * Send the given event to all regstered recipients, using a cached entry to minimize lookups.
  * This is particularly useful for optimizing sensors that frequently send to the same channel. 
  *
  * @param evt The event to send. This structure is assumed to be heap allocated, and will 
  * be automatically freed once all recipients have been notified.
  * @param c Cache entry to reduce lookups for commonly used channels.
  *
  * TODO: For now, this is unbuffered. We should consider scheduling events here, and operating
  * a different thread that empties the queue. This would perhaps provide greater opportunities
  * for aggregation.
  *
  * THIS IS NOW WRAPPED BY THE MicroBitEvent CLASS FOR CONVENIENCE...
  *
  * Example:
  * @code 
  * MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_DOWN,ticks,NULL);
  * evt.fire();
  * //OR YOU CAN DO THIS...  
  * MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_DOWN,ticks,NULL,true);
  * @endcode
  */
void MicroBitMessageBus::send(MicroBitEvent &evt, MicroBitMessageBusCache *c)
{
	MicroBitListener *l;
	MicroBitListener *start;

	// Find the start of the sublist where we'll send this event.
	// Ideally, we'll have a valid, cached entry. Use it if we do.
	if ( c != NULL && c->seq == this->seq)
	{
		l = c->ptr;
	}
	else
	{
		l = listeners;
		while (l != NULL && l->id != evt.source)
			l = l->next;
	}

	start = l;

	// Now, send the event to all listeners registered for this event.
	while (l != NULL && l->id == evt.source)
	{
		if(l->value ==  MICROBIT_EVT_ANY || l->value == evt.value)
		{
			create_fiber(l->cb);
		}

		l = l->next;
	}

	// Next, send to any listeners registered for ALL event sources.
	l = listeners;
	while (l != NULL && l->id == MICROBIT_ID_ANY)
	{
		create_fiber(l->cb);
		l = l->next;	
	}

	// Finally, if we were given a cached entry that's now invalid, update it.
	if ( c != NULL && c->seq != this->seq)
	{
		c->ptr = start;
		c->seq = this->seq;
	}
}

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
  *
  * TODO: We currently don't support C++ member functions as callbacks, which we should.
  *
  * Example:
  * @code 
  * void onButtonBClick()
  * {
  * 	//do something
  * }
  * uBit.MessageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBClick); // call function when ever a click event is detected.
  * @endcode
  */

void MicroBitMessageBus::listen(int id, int value, void (*handler)(void))
{
	//handler can't be NULL!
	if(handler == NULL)
		return;
	
	MicroBitListener *l, *p;
	MicroBitListener *newListener = new MicroBitListener(id, value, handler);

	//if listeners is null - we can automatically add this listener to the list at the beginning...
	if (listeners == NULL)
	{
		listeners = newListener;
		return;
	}

	// Firstly, we treat a listener as an idempotent operation. Ensure we don't already have this handler
	// registered in a that will already capture these events. If we do, silently ignore.
	l = listeners;

	while (l != NULL && l->id <= id)
	{
		if (l->cb == handler && (l->id == id || l->id == MICROBIT_ID_ANY) && (l->value == value || l->value == MICROBIT_EVT_ANY))
			return;

		l = l->next;
	}

	// Maintain an ordered list of listeners. 
	// Chain is held stictly in increasing order of ID (first level), then value code (second level).
	// Find the correct point in the chain for this event.
	// Adding a listener is a rare occurance, so we just walk the list.
	p = listeners;
	l = listeners;

	while (l != NULL && l->id < id && l->value <= value)
	{
		p = l;
		l = l->next;
	}

	//add in front of P
	if (p->id > id || p->value > value)
	{

		newListener->next = p;

		//this new listener is now the front!
		listeners = newListener;
	}
	//add after p
	else
	{
		newListener->next = p->next;
		p->next = newListener;
	}

	// Increase our sequence number and we're done.
	// This will lazily invalidate any cached entries to the listener list.
	this->seq++;
}

