#include "inc/MicroBitTest.h"

#ifdef MAIN_RANDOM_TEST

#include "MicroBit.h"

void
onLeftButton()
{
    char p[10] = "DICE: x";
    
    p[6] = (char)(uBit.random(5) + 49);
        
    ManagedString s(p);
   
    uBit.display.scrollStringAsync(s);
}

void app_main()
{   
#ifdef MICROBIT_DBG
    pc.printf("=== Random Numner Generator Test: BEGIN ===\n");
#endif    

    ManagedString intro("PRESS BUTTON TO ROLL DICE!");
    uBit.display.scrollString(intro);
    
    uBit.MessageBus.listen(MICROBIT_ID_LEFT_BUTTON, MICROBIT_BUTTON_EVT_DOWN, onLeftButton);
    
    while(1)
        uBit.sleep(1000);
}

#endif