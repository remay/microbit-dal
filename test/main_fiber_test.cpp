#include "inc/MicroBitTest.h"

#ifdef MAIN_FIBER_TEST

#include "MicroBit.h"

MicroBit uBit;
Serial pc(USBTX, USBRX);

void handler1()
{
    fiber_sleep(5000);
    pc.printf("handler1: X\n");   
}

void thread1()
{
    pc.printf("Thread1: Started\n");
 
    while(1)
    {
        pc.printf ("Thread1: Running...\n");
        fiber_sleep(10000);        
    }    
}

void thread2()
{
    pc.printf("Thread2: Started\n");
 
    while(1)
    {
        pc.printf ("Thread2: Running...\n");
        fiber_sleep(5000);        
    }    
}


int main()
{ 
    // Set up debug console.
    pc.baud (115200);
    
    // Initialize the Fiber scheduler
    scheduler_init();

    // 10 second delay just to let me get the terminal console window up. :-)
    fiber_sleep(10000);    

    // Create a few simple fibers, just for demonstration purposes.
    create_fiber(thread1);
    create_fiber(thread2);

    // Register an event handler, decoupled from a hard interrupt using a fiber.
    MicroBit::MessageBus.listen(MICROBIT_ID_LEFT_BUTTON, MICROBIT_BUTTON_EVT_DOWN, handler1);
    
    // Just hang out.
    // after scheduler_init() the main thread does have a fiber context, so can also sleep on the fiber scheduler.
    // power down will be handled by the idle task, in MicroBitFiber.cpp (detail still to do!).
    
    while(1)
    {
        fiber_sleep(10000);
        schedule();
    }
}


#endif