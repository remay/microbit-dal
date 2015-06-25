#include "MicroBit.h"

#ifdef MICROBIT_DBG
Serial pc(USBTX, USBRX);
#endif

MicroBit    uBit;

void
onResetButtonPressed(MicroBitEvent evt)
{
    NVIC_SystemReset();
}

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

    // Bring up core services.
    scheduler_init();
    uBit.init();
#ifdef MICROBIT_BLE
    // Test if we need to enter BLE pairing mode...
    int i=0;
    uBit.sleep(100);
    while (uBit.buttonA.isPressed() && uBit.buttonB.isPressed() && i<10)
    {
        uBit.sleep(100);
        i++;
        
        if (i == 10 && uBit.ble_firmware_update_service != NULL)
            uBit.ble_firmware_update_service->pair();
    }
#endif
        
    //uBit.MessageBus.listen(MICROBIT_ID_BUTTON_RESET, MICROBIT_BUTTON_EVT_CLICK, onResetButtonPressed);
    app_main();
    
    while(1)
        uBit.sleep(100);
        
#endif

}