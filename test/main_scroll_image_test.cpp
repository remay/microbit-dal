#include "inc/MicroBitTest.h"

#ifdef MAIN_SCROLL_IMAGE_TEST

#include "MicroBit.h"


void
onScrollImageComplete()
{
#ifdef MICROBIT_DBG
    pc.printf("=== onScrollImageComplete ===\n");
#endif     
}


void app_main()
{   
    uBit.MessageBus.listen(MICROBIT_ID_DISPLAY, MICROBIT_DISPLAY_EVT_SCROLLIMAGE_COMPLETE, onScrollImageComplete);
  
    // Setup a basic triangular waveform to test with.
    MicroBitImage img(20,5);

    int y = 0;    
    int step = 1;
    for (int x=0; x<20; x++)
    {
        img.setPixelValue(x,y,255);
        y += step;
        if(y >= 5 || y < 0)
        {
            step = -step;
            y += step;
        }
    }
     
    while(1)
    {
        uBit.display.scrollImage(img, MICROBIT_DEFAULT_SCROLL_SPEED, -1);
        uBit.display.scrollImage(img, MICROBIT_DEFAULT_SCROLL_SPEED*5, -5);

        uBit.display.scrollImage(img, MICROBIT_DEFAULT_SCROLL_SPEED, 1);
        uBit.display.scrollImage(img, MICROBIT_DEFAULT_SCROLL_SPEED*5, 5);
    }
}

#endif