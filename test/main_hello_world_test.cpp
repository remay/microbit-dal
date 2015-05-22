#include "inc/MicroBitTest.h"

#ifdef MAIN_HELLOWORLD_TEST

#include "MicroBit.h"

MicroBit uBit;
Serial pc(USBTX, USBRX);

char *defaultMessage = "HI HOWARD! WANT TO PLAY?";
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

int main()
{   
    pc.baud(115200);
 
    wait(10.0);
    
    pc.printf("=== Starting Display Test ===\n");
     
    uBit.init();

    ManagedString msg(defaultMessage); 
    while(1)
    {
        uBit.display.scrollString(msg);
        wait(15);
    }

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
}

#endif