#include "inc/MicroBit.h"


/**
  * Device level Message Bus abstraction
  */

void (*MicroBit::messageBus)(MicroBitEvent *) = NULL;



void MicroBit::init()
{

}



/**
  * Constructor. 
  * Create a representation of a MicroBit device.
  * @param messageBus callback function to receive MicroBitMessageBus events.
  */
MicroBit::MicroBit(void (*messageBus)(MicroBitEvent *)) :
  userLED(MICROBIT_ID_USER_LED, MICROBIT_PIN_USER_LED),
  display(MICROBIT_ID_DISPLAY, 5, 5)
  
{
    // Just store this for later.
    init();
    MicroBit::messageBus = messageBus;
}
