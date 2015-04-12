/**
  * Class definition for MicroBit Button.
  *
  * Represents a single button on the device.
  */
  
#ifndef MICROBIT_BUTTON_H
#define MICROBIT_BUTTON_H

class MicroBitButton
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
    int id;
    int state;
    
    public:
    /**
      * Constructor. 
      * Create a pin representation with the given ID.
      * @param id the ID of the new LED object.
      */
    MicroBitButton(int id, PinName pin);
    
    /**
      * Tests if this Button is currently pressed.
      * @return 1 if this button is pressed, 0 otherwise.
      */
    int isPressed();

     /**
      * Enables asynchronous callback events from this button.
      * When enabled, all state change updates will be propogated 
      * along the MicroBitMessageBus using the device's ID.
      */    
    void enableCallback();
    
     /**
      * Disables asynchronous callback events from this button.
      * When disabled no state change updates will be propogated 
      * along the MicroBitMessageBus from this button.
      */    
    void disableCallback();
    
};

#endif
