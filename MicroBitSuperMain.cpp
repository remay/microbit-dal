#include "MicroBit.h"

#ifdef MICROBIT_DBG
Serial pc(USBTX, USBRX);
#endif

MicroBit    uBit;

int main()
{   
#ifdef MICROBIT_DBG
    pc.baud(115200);

    // For diagnostics. Gives time to open the console window. :-) 
    for (int i=10; i>0; i--)
    {
        pc.printf("=== SUPERMAIN: Starting in %d ===\n", i);
        wait(1.0);
    }

    pc.printf("=== scheduler init... ");
    scheduler_init();
    pc.printf("complete ===\n");
        
    pc.printf("=== uBit init... ");
    uBit.init();
    pc.printf("complete ===\n");
    
    pc.printf("=== Launching app_main ===\n");
    app_main();

    pc.printf("=== app_main exited! ===\n");
    
    while(1)
        uBit.sleep(100);
    
#else

    scheduler_init();
    uBit.init();
    app_main();
    
    while(1)
        uBit.sleep(100);
        
#endif

}