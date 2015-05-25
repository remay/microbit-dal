#include "inc/MicroBitTest.h"

#ifdef MAIN_HELLOWORLD_TEST

#include "MicroBit.h"

struct QuarantinedBLEDevice
{
//    uint8_t     q1[256];
    BLEDevice   ble;
    uint8_t     q2[256];
};

Serial pc(USBTX, USBRX);

char *defaultMessage = "HI HOWARD! WANT TO PLAY?";
char bleMessage[50];
int update = 0;

char MICROBIT_BLE_DEVICE_NAME[] = "BBC MicroBit [xxxxx]";
char MICROBIT_BLE_MANUFACTURER[] = "The Cast of W1A";
char MICROBIT_BLE_MODEL[] = "Microbit SB2";
char MICROBIT_BLE_SERIAL[] = "SN1";
char MICROBIT_BLE_HARDWARE_VERSION[] = "0.2";
char MICROBIT_BLE_FIRMWARE_VERSION[] = "1.1";
char MICROBIT_BLE_SOFTWARE_VERSION[] = "1.0";

QuarantinedBLEDevice        *qble;
DeviceInformationService    *ble_device_information_service;
MicroBitDFUService          *ble_firmware_update_service;

MicroBit                    *microBit;


void
updateScroll(char* str, int len)
{
    len = min(len,50);
    memcpy(bleMessage, str, len);
    bleMessage[len]=0;
    update = 1;
}

void init_ble()
{
    // Start the BLE stack.        
    qble = new QuarantinedBLEDevice();
    //qble->ble = new BLEDevice();
    qble->ble.init();
 
    // Add our auxiliary services.
    ble_firmware_update_service = new MicroBitDFUService(qble->ble);
    ble_device_information_service = new DeviceInformationService(qble->ble, MICROBIT_BLE_MANUFACTURER, MICROBIT_BLE_MODEL, MICROBIT_BLE_SERIAL, MICROBIT_BLE_HARDWARE_VERSION, MICROBIT_BLE_FIRMWARE_VERSION, MICROBIT_BLE_SOFTWARE_VERSION);
    
    // Compute our auto-generated MicroBit device name.
    ble_firmware_update_service->getName(MICROBIT_BLE_DEVICE_NAME+14);
    
    // Setup advertising.
    qble->ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    qble->ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)MICROBIT_BLE_DEVICE_NAME, sizeof(MICROBIT_BLE_DEVICE_NAME));
    qble->ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    qble->ble.setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000));
    qble->ble.startAdvertising();   
}

int main()
{   
    init_ble();
        
    pc.baud(115200);
 
    wait(10.0);
    
    pc.printf("=== uBit init ===\n");
    microBit = new MicroBit();
    microBit->init();
    
    /*
    microBit->ble = &qble->ble;
    microBit->ble_device_information_service = ble_device_information_service;
    microBit->ble_firmware_update_service = ble_firmware_update_service;
    */
    pc.printf("=== uBit init complete ===\n");
      
    pc.printf("=== scheduler init ===\n");
    scheduler_init();
    pc.printf("=== scheduler init complete ===\n");

    pc.printf("=== display init ===\n");
    // Start the LED Matrix driver.
    uBit.display.startDisplay();
    pc.printf("=== display init complete ===\n");

    uBit.display.print('X');
    
    wait(3.0);
    
    pc.printf("=== Sleeping... ===\n");    
    wait(1.0);
    //fiber_sleep(1000);
    
    pc.printf("=== Sleeping... ===\n");    
    wait(1.0);    
    //fiber_sleep(1000);

    pc.printf("=== Sleeping... ===\n");    
    wait(1.0);
    //fiber_sleep(1000);

    pc.printf("=== Sleeping... ===\n");    
    wait(1.0);

    pc.printf("=== Sleeping... ===\n");    
    wait(1.0);

    ManagedString msg(defaultMessage); 
    while(1)
    {
        uBit.display.scrollStringAsync(msg);
        for (int i=0; i<150; i++)
        {
            wait(0.1);

/*    
            if (uBit.ble)
                uBit.ble->waitForEvent();
*/
        }
    }

/*
    // Enter Pairing mode if appropriate
    if(uBit.leftButton.isPressed())
        uBit.ble_firmware_update_service->pair();
 
 
    // Start main app.
    
    while(1)
    {
        if (update)
        {
            update = 0;
        }
        
        uBit.display.scrollString(msg);

        for (int i=0; i<150; i++)
        {
            wait(0.1);
                        
            if (uBit.ble)
                uBit.ble->waitForEvent();
            
            if (update)
                break;
        }
    }
*/
}

#endif