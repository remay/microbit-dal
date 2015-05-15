#include "inc/MicroBit.h"
#include "inc/MicroBitMagnetometer.h"
#include "inc/MicroBitMessageBus.h"

/**
  * Constructor. 
  * Create a magnetometer representation with the given ID.
  * @param id the ID of the new object.
  * @param address the I2C address of the device.
  */
MicroBitMagnetometer::MicroBitMagnetometer(int id, int address)
{
    this->id = id;
    this->address = address;
}

/**
  * Gets the current heading of the device, relative to magnetic north.
  * @return the current heading, in degrees.
  */
int MicroBitMagnetometer::heading()
{
    return 0;
}


