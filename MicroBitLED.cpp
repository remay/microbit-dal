
#include "inc/MicroBitLED.h"

       
/**
  * Constructor. 
  * Create an LED representation with the given ID.
  * @param id the ID of the new LED object.
  */
MicroBitLED::MicroBitLED(int id, PinName name) :
pin(name)

{   
    this->id = id;
}


 /**
  * Sets this LED to the brightness specified.
  * @param b The brightness to set the LED, in the range 0..255.
  */    
void MicroBitLED::setBrightness(int b)
{
    //sanity check
    if(b < 0 || b > 255)
        return;
        
    pin.write(b > 0);
}

 /**
  * Tests the brightness of this LED.
  * @return the brightness of this LED, in the range 0..255.
  */    
int MicroBitLED::getBrightness()
{
    return pin.read() == 0 ? 0 : 255;
}    

