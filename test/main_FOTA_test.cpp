#include "inc/MicroBitTest.h"

#ifdef MAIN_FOTA_TEST

#include "MicroBit.h"

extern "C" void bootloader_start(void);

MicroBit uBit;



Serial pc(USBTX, USBRX);
char version[] = "1.1";

void dfuCallbackFn()
{
    pc.printf("=== DFU Callback ===\n");   
}

int
ble_pair()
{
    pc.printf("=== Entering FOTA Pairing Mode ===\n");   
    while(1)
    {
        //uBit.ble->waitForEvent();
        wait(1.0);
    }
}

int main()
{ 
    // Set up debug console.
    pc.baud (115200);

    wait(10.0);
    
    pc.printf("=== Starting FOTA Test ===\n");
    for (int i=5; i>0; i--)
    {
        pc.printf("%d...\n",i);
        wait(1.0);
    }

    
    //if(uBit.leftButton.isPressed())
    //{
    //    ble_pair(); 
    //}    
    
    pc.printf("=== Starting Bootloader ===\n");    
    bootloader_start();
    
    pc.printf("=== Starting Application ===\n");    
    while(1)
    {
        //pc.printf("Running: %s\n", version);
        uBit.ble->waitForEvent();
    }
}

#endif