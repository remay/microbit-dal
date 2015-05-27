#include "inc/MicroBitTest.h"

#ifdef MAIN_HELLOWORLD_TEST

#include "MicroBit.h"

char *defaultMessage = "HI JAMES! WANT TO PLAY?";

void
onScrollTextComplete()
{
#ifdef MICROBIT_DBG    
    pc.printf("=== onScrollTextComplete ===\n");
#endif
    
    ManagedString s("BOING!! ");
    uBit.display.scrollStringAsync(s);    
}

void
onPrintTextComplete()
{
#ifdef MICROBIT_DBG    
    pc.printf("=== onPrintTextComplete ===\n");
#endif
    
    ManagedString s("PING!! ");
    uBit.display.scrollStringAsync(s);    
}

void
onLeftButton()
{    
    ManagedString s("LEFT!! ");
    uBit.display.scrollStringAsync(s);    
}

void
onRightButton()
{
    ManagedString s("RIGHT!! ");
    uBit.display.scrollStringAsync(s);    
}

void app_main()
{   
    uBit.MessageBus.listen(MICROBIT_ID_DISPLAY, MICROBIT_DISPLAY_EVT_SCROLLTEXT_COMPLETE, onScrollTextComplete);
    uBit.MessageBus.listen(MICROBIT_ID_DISPLAY, MICROBIT_DISPLAY_EVT_PRINTTEXT_COMPLETE, onPrintTextComplete);
    uBit.MessageBus.listen(MICROBIT_ID_LEFT_BUTTON, MICROBIT_BUTTON_EVT_DOWN, onLeftButton);
    uBit.MessageBus.listen(MICROBIT_ID_RIGHT_BUTTON, MICROBIT_BUTTON_EVT_DOWN, onRightButton);

    ManagedString msg(defaultMessage);
    uBit.display.scrollString(msg);
     
    while(1)
        uBit.sleep(100);        
}

#endif