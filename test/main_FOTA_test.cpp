#include "inc/MicroBitTest.h"

#ifdef MAIN_FOTA_TEST

#include "MicroBit.h"

char *defaultMessage = "FOTA TEST... ";
char bleMessage[50];
int update = 0;

void
updateScroll(char* str, int len)
{
    len = min(len,50);
    memcpy(bleMessage, str, len);
    bleMessage[len]=0;
    update = 1;
}

void app_main()
{         
    // Enter Pairing mode if appropriate     
    if(uBit.leftButton.isPressed())
        uBit.ble_firmware_update_service->pair();

    // Otherwise, just hang out and wait for a BLE request for FOTA programming.
    ManagedString msg(defaultMessage);

    uBit.display.scrollStringAsync(msg);
 
    while(1)
    {        
        uBit.sleep(100);
        if (update)
        {
            msg = ManagedString(bleMessage);
            uBit.display.scrollStringAsync(msg);    
            update = 0;   
        }
    }
}

#endif