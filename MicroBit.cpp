#include "inc/MicroBit.h"
#include "URIBeaconConfigService.h"

/**
  * Constructor. 
  * Create a representation of a MicroBit device.
  * @param messageBus callback function to receive MicroBitMessageBus events.
  */
MicroBit::MicroBit() : 
    MessageBus(), 
    display(MICROBIT_ID_DISPLAY, 5, 5)
    //leftButton(MICROBIT_ID_LEFT_BUTTON,MICROBIT_PIN_LEFT_BUTTON),
    //rightButton(MICROBIT_ID_RIGHT_BUTTON,MICROBIT_PIN_RIGHT_BUTTON),
    //i2c(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL)
{   
    flags = 0x00;
}

/**
  * Post constructor initialisation method.
  * After *MUCH* pain, it's noted that the BLE stack can't be brought up in a 
  * static context, so we bring it up here rather than in the constructor.
  * n.b. This method *must* be called in main() or later, not before.
  */
void MicroBit::init()
{
}

/**
  * Delay for the givne amount of time.
  * If the scheduler is running, this will deschedule the current fiber and perform
  * a power efficent, concurrent sleep operation.
  * If the scheduler is disabled or we're running in an interrupt context, this
  * will revert to a busy wait. 
  */
void MicroBit::sleep(int milliseconds)
{
    if (flags & MICROBIT_FLAG_SCHEDULER_RUNNING)
        fiber_sleep(milliseconds);
    else
        wait_ms(milliseconds);
}