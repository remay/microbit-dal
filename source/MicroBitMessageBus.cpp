/**
  * Class definition for a MicroBitMessageBus.
  *
  * The MicroBitMessageBus handles all messages passed between components.
  */

#include "MicroBit.h"

/**
  * Constructor. 
  * Create a new Message Bus.
  */
MicroBitMessageBus::MicroBitMessageBus()
{
	this->listeners = NULL;
    this->evt_queue_head = NULL;
    this->evt_queue_tail = NULL;
    this->nonce_val = 0;
}

/**
 * Returns a 'nonce' for use with the NONCE_ID channel of the message bus.
 */
uint16_t MicroBitMessageBus::nonce()
{
    // In the global scheme of things, a terrible nonce generator. 
    // However, for our purposes, this is simple and adequate for local use.
    // This would be a bad idea if our events were networked though - can you think why?
    return nonce_val++;
}

/**
  * Invokes a callback on a given MicroBitListener
  *
  * Internal wrapper function, used to enable
  * parameterised callbacks through the fiber scheduler.
  */
void async_callback(void *param)
{
	MicroBitListener *listener = (MicroBitListener *)param;

    // OK, now we need to decide how to behave depending on our configuration.
    // If this a fiber f already active within this listener then check our
    // configuration to determine the correct course of action. 
    //

    if (listener->flags & MESSAGE_BUS_LISTENER_BUSY)
    {
        // Drop this event, if that's how we've been configured.
        if (listener->flags & MESSAGE_BUS_LISTENER_DROP_IF_BUSY)
            return;

        // Queue this event up for later, if that's how we've been configured.
        if (listener->flags & MESSAGE_BUS_LISTENER_QUEUE_IF_BUSY)
        {
            listener->queue(listener->evt);
            return;
        }
    }

    // Determine the calling convention for the callback, and invoke... 
    // C++ is really bad at this! Especially as the ARM compiler is yet to support C++ 11 :-/

    // Record that we have a fiber going into this listener...
    listener->flags |= MESSAGE_BUS_LISTENER_BUSY;

    while (1)
    {
        // Firstly, check for a method callback into an object.
        if (listener->flags & MESSAGE_BUS_LISTENER_METHOD)
            listener->cb_method->fire(listener->evt);

        // Now a parameterised C function
        else if (listener->flags & MESSAGE_BUS_LISTENER_PARAMETERISED)
            listener->cb_param(listener->evt, listener->cb_arg);

        // We must have a plain C function
        else
            listener->cb(listener->evt);

        // If there are more events to process, dequeue te next one and process it.
        if ((listener->flags & MESSAGE_BUS_LISTENER_QUEUE_IF_BUSY) && listener->evt_queue)
        {
            MicroBitEventQueueItem *item = listener->evt_queue;

            listener->evt = item->evt;
            listener->evt_queue = listener->evt_queue->next;
            delete item;
        }
        else
            break;
    }

    // The fiber of exiting... clear our state.
    listener->flags &= ~MESSAGE_BUS_LISTENER_BUSY;
}


/**
  * Queue the given event for processing at a later time.
  * Add the given event at the tail of our queue.
  *
  * @param The event to queue. 
  */
void MicroBitMessageBus::queueEvent(MicroBitEvent &evt)
{
    int processingComplete;

    // Firstly, process all handler regsitered as URGENT. These pre-empt the queue, and are useful for fast, high priority services.
    processingComplete = this->process(evt, MESSAGE_BUS_LISTENER_URGENT);

    if (!processingComplete)
    {
        // We need to queue this event for later processing...
        MicroBitEventQueueItem *item = new MicroBitEventQueueItem(evt);

        __disable_irq();

        if (evt_queue_tail == NULL)
            evt_queue_head = evt_queue_tail = item;
        else 
            evt_queue_tail->next = item;

        __enable_irq();
    }
}

/**
  * Extract the next event from the front of the event queue (if present).
  * @return 
  *
  * @param The event to queue. 
  */
MicroBitEventQueueItem* MicroBitMessageBus::dequeueEvent()
{
    MicroBitEventQueueItem *item = NULL;

    __disable_irq();
 
    if (evt_queue_head != NULL)
    { 
        item = evt_queue_head;
        evt_queue_head = item->next;

        if (evt_queue_head == NULL)
            evt_queue_tail = NULL;
    }
    
    __enable_irq();

    return item;
}

/**
  * Periodic callback from MicroBit.
  * Process at least one event from the event queue, if it is not empty.
  * We then continue processing events until something appears on the runqueue.
  */  
void MicroBitMessageBus::idleTick()
{
    MicroBitEventQueueItem *item = this->dequeueEvent();

    // Whilst there are events to process and we have no useful other work to do, pull them off the queue and process them.
    while (item)
    {
        // send the event to all standard event listeners.
        this->process(item->evt);

        // Free the queue item.
        delete item;

        // If we have created some useful work to do, we stop processing.
        // This helps to minimise the number of blocked fibers we create at any point in time, therefore
        // also reducing the RAM footprint. 
        if(!scheduler_runqueue_empty())
            break;

        // Pull the next event to process, if there is one.
        item = this->dequeueEvent();
    }
}

/**
  * Indicates whether or not we have any background work to do.
  * @ return 1 if there are any events waitingto be processed, 0 otherwise. 
  */
int MicroBitMessageBus::isIdleCallbackNeeded()
{
    return !(evt_queue_head == NULL);  
}

/**
  * Queues the given event to be sent to all registered recipients.
  *
  * @param The event to send. 
  *
  * n.b. THIS IS NOW WRAPPED BY THE MicroBitEvent CLASS FOR CONVENIENCE...
  *
  * Example:
  * @code 
  * MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_DOWN,ticks,CREATE_ONLY);
  * evt.fire();
  *
  * //OR YOU CAN DO THIS...  
  * MicroBitEvent evt(id,MICROBIT_BUTTON_EVT_DOWN);
  * @endcode
  */
void MicroBitMessageBus::send(MicroBitEvent evt)
{
    // We simply queue processing of the event until we're scheduled in normal thread context.
    // We do this to avoid the possibility of executing event handler code in IRQ context, which may bring
    // hidden race conditions to kids code. Queuing all events ensures causal ordering (total ordering in fact).

    this->queueEvent(evt);
    return;
}

/*
 * Deliver the given event to all registered event handlers.
 * Event handlers are called using the invoke() mechanism provided by the fier scheduler
 * This will attempt to call the event handler directly, but spawn a fiber should that
 * event handler attempt a blocking operation.
 * @param evt The event to be delivered.
 * @param mask The type of listeners to process (optional). Matches MicroBitListener flags. If not defined, all standard listeners will be processed.
 * @return The 1 if all matching listeners were processed, 0 if further processing is required.
 */
int MicroBitMessageBus::process(MicroBitEvent &evt, uint32_t mask)
{
	MicroBitListener *l;
    int complete = 1;

    l = listeners;
    while (l != NULL)
    {
	    if((l->id == evt.source || l->id == MICROBIT_ID_ANY) && (l->value == evt.value || l->value == MICROBIT_EVT_ANY))
        {
            if(l->flags & mask) 
            {
                l->evt = evt;

                // OK, if this handler has regisitered itself as non-blocking, we just execute it directly... 
                // This is normally only done for trusted system components.
                // Otherwise, we invoke it in a 'fork on block' context, that will automatically create a fiber
                // should the event handler attempt a blocking operation, but doesn't have the overhead
                // of creating a fiber needlessly. (cool huh?)
                if (l->flags & MESSAGE_BUS_LISTENER_NONBLOCKING)
                    async_callback(l);
                else
                    invoke(async_callback, l);
            }
            else
            {
                complete = 0;
            }
		}

		l = l->next;
	}

    return complete;
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
  * Example:
  * @code 
  * void onButtonBClick(MicroBitEvent evt)
  * {
  * 	//do something
  * }
  * uBit.MessageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBClick); // call function when ever a click event is detected.
  * @endcode
  */

void MicroBitMessageBus::listen(int id, int value, void (*handler)(MicroBitEvent), uint16_t flags) 
{
	if (handler == NULL)
		return;

	MicroBitListener *newListener = new MicroBitListener(id, value, handler, flags);

    if(!add(newListener))
        delete newListener;
}

void MicroBitMessageBus::listen(int id, int value, void (*handler)(MicroBitEvent, void*), void* arg, uint16_t flags) 
{
	if (handler == NULL)
		return;

	MicroBitListener *newListener = new MicroBitListener(id, value, handler, arg, flags);

    if(!add(newListener))
        delete newListener;
}

/**
 * Unregister a listener function.
 * Listners are identified by the Event ID, Event VALUE and handler registered using listen().
 * 
 * @param id The Event ID used to register the listener.
 * @param value The Event VALUE used to register the listener.
 * @param handler The function used to register the listener.
 *
 *
 * Example:
 * @code 
 * void onButtonBClick()
 * {
 * 	//do something
 * }
 *
 * uBit.MessageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBClick); 
 * @endcode
 */
void MicroBitMessageBus::ignore(int id, int value, void (*handler)(MicroBitEvent))
{
	if (handler == NULL)
		return;

	MicroBitListener listener(id, value, handler);
    remove(&listener);
}

/**
 * Unregister a listener function.
 * Listners are identified by the Event ID, Event VALUE and handler registered using listen().
 * 
 * @param id The Event ID used to register the listener.
 * @param value The Event VALUE used to register the listener.
 * @param handler The function used to register the listener.
 *
 *
 * Example:
 * @code 
 * void onButtonBClick(void *arg)
 * {
 * 	//do something
 * }
 *
 * uBit.MessageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBClick); 
 * @endcode
 */
void MicroBitMessageBus::ignore(int id, int value, void (*handler)(MicroBitEvent, void*))
{
	if (handler == NULL)
		return;

    // The remove function is not comparing the [arg] anyhow.
	MicroBitListener listener(id, value, handler, NULL);
    remove(&listener);
}


/**
  * Add the given MicroBitListener to the list of event handlers, unconditionally.
  * @param listener The MicroBitListener to validate.
  * @return 1 if the listener is valid, 0 otherwise.
  */
int MicroBitMessageBus::add(MicroBitListener *newListener)
{
	MicroBitListener *l, *p;
    int methodCallback;

	//handler can't be NULL!
	if (newListener == NULL)
		return 0;

	l = listeners;

	// Firstly, we treat a listener as an idempotent operation. Ensure we don't already have this handler
	// registered in a that will already capture these events. If we do, silently ignore.

    // We always check the ID, VALUE and CB_METHOD fields.
    // If we have a callback to a method, check the cb_method class. Otherwise, the cb function point is sufficient.
    while (l != NULL)
    {
        methodCallback = (newListener->flags & MESSAGE_BUS_LISTENER_METHOD) && (l->flags & MESSAGE_BUS_LISTENER_METHOD);

        if (l->id == newListener->id && l->value == newListener->value && (methodCallback ? *l->cb_method == *newListener->cb_method : l->cb == newListener->cb))
            return 0;

        l = l->next;
    }

    // We have a valid, new event handler. Add it to the list.
	// if listeners is null - we can automatically add this listener to the list at the beginning...
	if (listeners == NULL)
	{
		listeners = newListener;
		return 1;
	}

	// We maintain an ordered list of listeners. 
	// The chain is held stictly in increasing order of ID (first level), then value code (second level).
	// Find the correct point in the chain for this event.
	// Adding a listener is a rare occurance, so we just walk the list...

	p = listeners;
	l = listeners;

	while (l != NULL && l->id < newListener->id)
	{
		p = l;
		l = l->next;
	}

	while (l != NULL && l->id == newListener->id && l->value < newListener->value)
	{
		p = l;
		l = l->next;
	}

	//add at front of list
	if (p == listeners && (newListener->id < p->id || (p->id == newListener->id && p->value > newListener->value)))
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

    return 1;
}

/**
  * Remove a MicroBitListener from the list that matches the given listener.
  * @param listener The MicroBitListener to validate.
  * @return The number of listeners removed from the list.
  */
int MicroBitMessageBus::remove(MicroBitListener *listener)
{
	MicroBitListener *l, *p;
    int removed = 0;

	//handler can't be NULL!
	if (listener == NULL)
		return 0;

	l = listeners;
	p = NULL;

    // Walk this list of event handlers. Delete any that match the given listener.
    while (l != NULL)
    {
        if ((listener->flags & MESSAGE_BUS_LISTENER_METHOD) == (l->flags & MESSAGE_BUS_LISTENER_METHOD))
        {
            if(((listener->flags & MESSAGE_BUS_LISTENER_METHOD) && (*l->cb_method == *listener->cb_method)) || 
                    ((!(listener->flags & MESSAGE_BUS_LISTENER_METHOD) && l->cb == listener->cb)))
            {
                if ((listener->id == MICROBIT_ID_ANY || listener->id == l->id) && (listener->value == MICROBIT_EVT_ANY || listener->value == l->value))
                {
                    // Found a match. Remove from the list.
                    if (p == NULL)
                        listeners = l->next;
                    else 
                        p->next = l->next;

                    // delete the listener.
                    MicroBitListener *t = l;
                    l = l->next;

                    delete t;
                    removed++;

                    continue;
                }
            }
        }

        p = l;
        l = l->next;
    }

    return removed;
}

/**
 * Returns the microBitListener with the given position in our list.
 * @param n The position in the list to return.
 * @return the MicroBitListener at postion n in the list, or NULL if the position is invalid.
 */
MicroBitListener* MicroBitMessageBus::elementAt(int n)
{
    MicroBitListener *l = listeners;

    while (n > 0)
    {
        if (l == NULL)
            return NULL;

        n--;
        l = l->next;
    }

    return l;
}

