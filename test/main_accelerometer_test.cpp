#include "inc/MicroBitTest.h"

#ifdef MAIN_ACCELEROMETER_TEST

#include "MicroBit.h"
#include "MicroBitAccelerometer.h"

void app_main()
{   
    int x, y;
    
#ifdef MICROBIT_DBG
    pc.printf("=== Accelerometer Test: BEGIN ===\n");
#endif     
    
    if(uBit.accelerometer.whoAmI() == MMA8653_WHOAMI_VAL)
    {
        ManagedString s("ACCEL FOUND = YES");
        uBit.display.scrollString(s);
    }
    else
    {
        ManagedString s("ACCEL FOUND = NO");
        uBit.display.scrollString(s);

        while (1)
            uBit.sleep(200);
    }
    
    while(1)
    {
        x = 0;
        y = 0;
        
        if (uBit.accelerometer.getX() < 750)
            x++;
        if (uBit.accelerometer.getX() < 250)
            x++;
        if (uBit.accelerometer.getX() < -250)
            x++;
        if (uBit.accelerometer.getX() < -750)
            x++;
            
        if (uBit.accelerometer.getY() < 750)
            y++;
        if (uBit.accelerometer.getY() < 250)
            y++;
        if (uBit.accelerometer.getY() < -250)
            y++;
        if (uBit.accelerometer.getY() < -750)
            y++;
        
        uBit.display.image.clear();
        uBit.display.image.setPixelValue(x,y,255);
        
        uBit.sleep(100);
    }
}

#endif