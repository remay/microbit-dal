#include "inc/MicroBit.h"


/**
  * Device level Message Bus abstraction
  */

MicroBitMessageBus MicroBit::MessageBus;

void MicroBit::init()
{

}



/**
  * Constructor. 
  * Create a representation of a MicroBit device.
  * @param messageBus callback function to receive MicroBitMessageBus events.
  */
MicroBit::MicroBit() :
  userLED(MICROBIT_ID_USER_LED, MICROBIT_PIN_USER_LED),
  display(MICROBIT_ID_DISPLAY, 5, 5),
  leftButton(MICROBIT_ID_LEFT_BUTTON,MICROBIT_PIN_LEFT_BUTTON)
  
{
    init();
}
