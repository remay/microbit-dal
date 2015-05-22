/**
  * The MicroBit Fiber scheduler.
  *
  * This lightweight, non-preemptive scheduler provides a simple threading mechanism for two main purposes:
  *
  * 1) To provide a clean abstraction for application languages to use when building async behaviour (callbacks).
  * 2) To provide ISR decoupling for Messagebus events generted in an ISR context.
  */
  
#include "MicroBit.h"

/*
 * Statically allocated values used to create and destroy Fibers.
 * required to be defined here to allow persistence during context switches.
 */
 
/*
 * Scheduler state.
 */
 
Fiber *currentFiber = NULL;                 // The context in which the current fiber is executing.
Fiber *runQueue = NULL;                     // The list of runnable fibers.
Fiber *sleepQueue = NULL;                   // The list of blocked fibers waiting on a fiber_sleep() operation.
Fiber *idle = NULL;                         // IDLE task - performs a power efficient sleep.
Fiber *fiberPool = NULL;                    // Pool of unused fibers, just waiting for a job to do.

Cortex_M0_TCB  *emptyContext = NULL;        // Initialized context for fiber entry state.

/*
 * Time since power on. Measured in milliseconds.
 * When stored as an unsigned long, this gives us approx 50 days between rollover, which is ample. :-)
 */
unsigned long ticks = 0;

/**
  * Utility function to add the currenty running fiber to the given queue. 
  * Perform a simple add at the head, to avoid complexity,
  * Queues are normally very short, so maintaining a doubly linked, sorted list typically outweighs the cost of
  * brute force searching.
  *
  * @param f The fiber to add to the queue
  * @param queue The run queue to add the fiber to.
  */
void queue_fiber(Fiber *f, Fiber **queue)
{
    __disable_irq();

    f->queue = queue;
    f->next = *queue;
    f->prev = NULL;
    
    if(*queue != NULL)
        (*queue)->prev = f;
        
    *queue = f;    

    __enable_irq();
}

/**
  * Utility function to the given fiber from whichever queue it is currently stored on. 
  * @param f the fiber to remove.
  */
void dequeue_fiber(Fiber *f)
{
    __disable_irq();
    
    if (f->prev != NULL)
        f->prev->next = f->next;
    else
        *(f->queue) = f->next;
    
    if(f->next)
        f->next->prev = f->prev;
        
    f->next = NULL;
    f->prev = NULL;
    f->queue = NULL;
    
    __enable_irq();

}


/**
  * Initialises the Fiber scheduler. 
  * Creates a Fiber context around the calling thread, and adds it to the run queue as the current thread.
  *
  * This function must be called once only from the main thread, and before any other Fiber operation.
  */
void scheduler_init()
{
    // Create a new fiber context
    currentFiber = new Fiber();
    currentFiber->stack_bottom = (uint32_t) malloc(FIBER_STACK_SIZE);
    currentFiber->stack_top = ((uint32_t) currentFiber->stack_bottom) + FIBER_STACK_SIZE;
    
    // Add ourselves to the run queue.
    queue_fiber(currentFiber, &runQueue);
 
    // Build a fiber context around the current thread.
    swap_context(&currentFiber->tcb, &currentFiber->tcb, currentFiber->stack_top, currentFiber->stack_top);    
    
    // Create the IDLE task. This will actually scheduk the IDLE task, but it will immediately yeld back to us.
    // Remove it from the run queue though, as IDLE is a special case.
    idle = create_fiber(idle_task);    
    dequeue_fiber(idle);    
    
    // Flag that we now have a scheduler running
    uBit.flags |= MICROBIT_FLAG_SCHEDULER_RUNNING;
}

/**
  * Timer callback. Called from interrupt context, once every FIBER_TICK_PERIOD_MS milliseconds.
  * Simply checks to determine if any fibers blocked on the sleep queue need to be woken up 
  * and made runnable.
  */
void scheduler_tick()
{
    Fiber *f = sleepQueue;
    Fiber *t;
    
    // increment our real-time counter.
    ticks += FIBER_TICK_PERIOD_MS;
    
    // Check the sleep queue, and wake up any fibers as necessary.
    while (f != NULL)
    {
        t = f->next;        
        
        if (ticks >= f->context)
        {
            // Wakey wakey!
            dequeue_fiber(f);
            queue_fiber(f,&runQueue);
        }
        
        f = t;
    }
}

/**
  * Blocks the calling thread for the given period of time.
  * The calling thread will be immediatley descheduled, and placed onto a 
  * wait queue until the requested amount of time has elapsed. 
  * 
  * n.b. the fiber will not be be made runnable until after the elasped time, but there
  * are no guarantees precisely when the fiber will next be scheduled.
  *
  * @param t The period of time to sleep, in milliseconds.
  */
void fiber_sleep(unsigned long t)
{
    // Calculate and store the time we want to wake up.
    currentFiber->context = ticks + t;
    
    // Remove ourselve from the run queue
    dequeue_fiber(currentFiber);
        
    // Add ourselves to the sleep queue. We maintain strict ordering here to reduce lookup times.
    queue_fiber(currentFiber, &sleepQueue);
    
    // Finally, enter the scheduler.
    schedule();
}

Fiber *getFiberContext()
{
    Fiber *f;
    
    __disable_irq();
        
    if (fiberPool != NULL)
    {
        f = fiberPool;
        dequeue_fiber(f);
        
        // dequeue_fiber() exits with irqs enablesd, so no need to do this again!
    }
    else
    {
        __enable_irq();
        
        f = new Fiber();
        f->stack_bottom = (uint32_t) malloc(FIBER_STACK_SIZE);
        f->stack_top = f->stack_bottom + FIBER_STACK_SIZE;
    }    
    
    return f;
}

/**
 * Creates a new Fiber, and launches it.
  *
  * @param entry_fn The function the new Fiber will begin execution in.
  * @return The new Fiber.
  */
Fiber *create_fiber(void (*entry_fn)(void))
{
    Fiber *newFiber = getFiberContext();
    *((uint32_t *)newFiber->stack_bottom) = (uint32_t) entry_fn;

    // Use cache fiber state if we have it. This is faster, and safer if we're called from
    // an interrupt context.
    if (emptyContext != NULL)
    {
        memcpy(&newFiber->tcb, emptyContext, sizeof(Cortex_M0_TCB));
    }
    else
    {
        // Otherwise, initialize the TCB from the current context.
        save_context(&newFiber->tcb, newFiber->stack_top);
        
        // Assign the new stack pointer and entry point.
        newFiber->tcb.SP = CORTEX_M0_STACK_BASE;    
        newFiber->tcb.LR = (uint32_t) &&LAUNCH_NEW_FIBER;
        
        // Store this context for later use.
        emptyContext = (Cortex_M0_TCB *) malloc (sizeof(Cortex_M0_TCB));
        memcpy(emptyContext, &newFiber->tcb, sizeof(Cortex_M0_TCB));
    }
    
    // Add new fiber to the run queue.
    queue_fiber(newFiber, &runQueue);
        
    return newFiber;

LAUNCH_NEW_FIBER:     
 
    // Launch into the entry point, now we're in the correct context. 
    (*(void(*)())(*((uint32_t *)currentFiber->stack_bottom)))();
    
    // Garbage collect the fiber once it exits.    
    release_fiber();
        
    return NULL;
}


/**
  * Exit point for all fibers.
  * Any fiber reaching the end of its entry function will return here  for recycling.
  *
  * TODO: Build a Fiber Pool, rather than just leak memory here. :-)
  */
void release_fiber(void)
{  
    // Remove ourselves form the runqueue.
    dequeue_fiber(currentFiber);
    queue_fiber(currentFiber, &fiberPool);
    
    // Find something else to do!
    schedule();   
}

/**
  * Calls the Fiber scheduler.
  * The calling Fiber will likely be blocked, and control given to another waiting fiber.
  * Call this to yield control of the processor when you have nothing more to do.
  */
void schedule()
{
    // Just round robin for now!
    Fiber *oldFiber = currentFiber;

    // OK - if we've nothing to do, then run the IDLE task (power saving sleep)
    if (runQueue == NULL)
        currentFiber = idle;
        
    // If the current fiber is on the run queue, round robin.
    else if (currentFiber->queue == &runQueue)
        currentFiber = currentFiber->next == NULL ? runQueue : currentFiber->next;

    // Otherwise, just pick the head of the run queue.
    else
        currentFiber = runQueue;
        
    // Swap to the context of the chosen fiber, and we're done.
    // Don't bother with the overhead of switching if there's only one fiber on the runqueue!
    if (currentFiber != oldFiber)        
        swap_context(&oldFiber->tcb, &currentFiber->tcb, oldFiber->stack_top, currentFiber->stack_top);
}

/**
  * IDLE task.
  * Only scheduled for execution when the runqueue is empty.
  * Performs a procressor sleep operation, then returns to the scheduler - most likely after a timer interrupt.
  */
void idle_task()
{
    while(1)
    {
        // TODO: Efficient sleep goes here. :-)
        //sleep();
        
        schedule();
    }
}
