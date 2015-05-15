#include "inc/MicroBit.h"
#include "inc/MicroBitButton.h"
#include "inc/MicroBitMessageBus.h"
/**
  * Constructor. 
  * Create a button representation with the given ID.
  * @param id the ID of the new LED object.
  * @param name the physical pin on the processor that this butotn is connected to.
  */
MicroBitButton::MicroBitButton(int id, PinName name) : pin(name), irq(name)
{
    this->id = id;
    this->name = name;
    irq.rise(this, &MicroBitButton::rising);
    irq.fall(this, &MicroBitButton::falling);
}

/**
  * Interrupt on change handler for this button.
  */
void MicroBitButton::rising()
{
    // TODO: FIX THIS. This many of the mbed libraries aren't re-entrant...
    MicroBitEvent *evt = new MicroBitEvent();
    evt->source = id;
    evt->context = NULL;
    evt->timestamp = 0;
    evt->value = MICROBIT_BUTTON_EVT_UP;
    
    uBit.MessageBus.send(evt);        
}

/**
  * Interrupt on change handler for this button.
  */
void MicroBitButton::falling()
{
    // TODO: FIX THIS. This many of the mbed libraries aren't re-entrant...
    MicroBitEvent *evt = new MicroBitEvent();
    evt->source = id;
    evt->context = NULL;
    evt->timestamp = 0;
    evt->value = MICROBIT_BUTTON_EVT_DOWN;
    
    uBit.MessageBus.send(evt);        
}


/**
  * Tests if this Button is currently pressed.
  * @return 1 if this button is pressed, 0 otherwise.
  */
int MicroBitButton::isPressed()
{
    return !pin;
}

