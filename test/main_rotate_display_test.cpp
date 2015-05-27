#include "inc/MicroBitTest.h"

#ifdef MAIN_ROTATE_DISPLAY_TEST

#include "MicroBit.h"

void app_main()
{   
#ifdef MICROBIT_DBG
    pc.printf("=== Random Numner Generator Test: BEGIN ===\n");
#endif    

    uBit.display.print('J');
    
    while(1)
    {
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_0);        
        uBit.sleep(1000);
            
        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_90);        
        uBit.sleep(1000);

        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_180);        
        uBit.sleep(1000);

        uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_270);        
        uBit.sleep(1000);
    }
}

#endif