#include "MicroBit.h"

MicroBit        uBit;
InterruptIn     resetButton(MICROBIT_PIN_BUTTON_RESET);

extern char* MICROBIT_BLE_DEVICE_NAME;

int main()
{    
    // Bring up soft reset button.
    resetButton.mode(PullUp);
    resetButton.fall(microbit_reset);
    
#if CONFIG_ENABLED(MICROBIT_DBG)

    // For diagnostics. Gives time to open the console window. :-) 
    uBit.serial.baud(115200);
    for (int i=3; i>0; i--)
    {
        uBit.serial.printf("=== SUPERMAIN: Starting in %d ===\n", i);
        wait(1.0);
    }

    uBit.serial.printf("micro:bit runtime DAL version %s\n", MICROBIT_DAL_VERSION);

#endif    

    // Bring up our nested heap allocator.
    microbit_heap_init();

    // Bring up fiber scheduler
    scheduler_init();
    
    // Bring up random number generator, BLE, display and system timers.    
    uBit.init();

    // Provide time for all threaded initialisers to complete.
    uBit.sleep(100);

#if CONFIG_ENABLED(MICROBIT_BLE_BLUEZONE)
    // Test if we need to enter BLE pairing mode...
    int i=0;
    while (uBit.buttonA.isPressed() && uBit.buttonB.isPressed() && i<10)
    {
        uBit.sleep(100);
        i++;
        
        if (i == 10)
        {
            // OK - we need to enter BLUE ZONE mode.
            // Test to see if BLE and the necessary services have been brought up already.
            // If not, start them.
            if (!uBit.ble)
            {
                uBit.ble = new BLEDevice();
                uBit.ble->init();
                uBit.ble->onDisconnection(bleDisconnectionCallback);

                // Ensure we're advertising.
                uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
                uBit.ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)MICROBIT_BLE_DEVICE_NAME, sizeof(MICROBIT_BLE_DEVICE_NAME));
                uBit.ble->setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
                uBit.ble->setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(200));
                uBit.ble->startAdvertising();  
            }

            if (!uBit.ble_firmware_update_service)
                uBit.ble_firmware_update_service = new MicroBitDFUService(*uBit.ble);

            // enter BLUE ZONE mode.
            uBit.ble_firmware_update_service->pair();
        }
    }
#endif
       
    app_main();

    // If app_main exits, there may still be other fibers running, registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
   
    // We should never get here, but just in case.
    while(1);
}
