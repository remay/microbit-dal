/**
  * Class definition for MicroBit LED.
  *
  * n.b. This is disjoint from the Display class,
  * and used only to represent the two stsus LEDs.
  */
  
#ifndef MICROBIT_LED_H
#define MICROBIT_LED_H

#include "mbed.h"

class MicroBitLED
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
    int id;
    DigitalOut pin;
    
    public:
    /**
      * Constructor. 
      * Create an LED representation with the given ID.
      * @param id the ID of the new LED object.
      */
    MicroBitLED(int id, PinName pin);

     /**
      * Sets this LED to the brightness specified.
      * @param b The brightness to set the LED, in the range 0..255.
      */    
    void setBrightness(int b);
    
     /**
      * Tests the brightness of this LED.
      * @return the brightness of this LED, in the range 0..255.
      */    
    int getBrightness();    
};

#endif

