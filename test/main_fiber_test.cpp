#include "inc/MicroBitTest.h"

#ifdef MAIN_FIBER_TEST

#include "MicroBit.h"

void handler1()
{
    unsigned char something[100];
    int t = 1;
       
    for (int i=0; i<100; i++)
        something[i] = i;
        
    fiber_sleep(5000);
    
    for (int i=0; i<100; i++)
        if (something[i] != i)
            t = 0;
            
#ifdef MICROBIT_DBG
    pc.printf("handler1: %d\n", t);   
#endif
}

void thread1()
{
#ifdef MICROBIT_DBG
    pc.printf("Thread1: Started\n");
#endif 
    while(1)
    {
 #ifdef MICROBIT_DBG
        pc.printf ("Thread1: Running...\n");
 #endif       
        fiber_sleep(10000);        
    }    
}

void thread2()
{
#ifdef MICROBIT_DBG    
    pc.printf("Thread2: Started\n");
#endif 
    while(1)
    {
#ifdef MICROBIT_DBG        
        pc.printf ("Thread2: Running...\n");
#endif        
        fiber_sleep(5000);        
    }    
}

void app_main()
{ 

    // Create a few simple fibers, just for demonstration purposes.
    create_fiber(thread1);
    create_fiber(thread2);

    // Register an event handler, decoupled from a hard interrupt using a fiber.
    uBit.MessageBus.listen(MICROBIT_ID_LEFT_BUTTON, MICROBIT_BUTTON_EVT_DOWN, handler1);
    
    // Just hang out.
    // after scheduler_init() the main thread does have a fiber context, so can also sleep on the fiber scheduler.
    // power down will be handled by the idle task, in MicroBitFiber.cpp.
    
    while(1)
        uBit.sleep(1000);
}

#endif