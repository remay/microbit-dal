/**
  * Class definition for MicroBit Accelerometer.
  *
  * Represents an implementation of the Freescale MMA8652 3 axis accelerometer
  * Also includes basic data caching and on demand activation.
  */
  
#include "MicroBit.h"

/**
  * Issues a standard, 2 byte I2C command write to the accelerometer.
  * Blocks the calling thread until complete.
  *
  * @param reg The address of the register to write to.
  * @param value The value to write.
  */
void MicroBitAccelerometer::writeCommand(uint8_t reg, uint8_t value)
{
    uint8_t command[2];
    command[0] = reg;
    command[1] = value;
    
    //uBit.i2c.write(address, (const char *)command, 2);
}

/**
  * Issues a read command into the specified buffer.
  * Blocks the calling thread until complete.
  *
  * @param reg The address of the register to access.
  * @param buffer Memory area to read the data into.
  * @param length The number of bytes to read.
  */
void MicroBitAccelerometer::readCommand(uint8_t reg, uint8_t* buffer, int length)
{
    //uBit.i2c.write(address, (const char *)&reg, 1, true);
    //uBit.i2c.read(address, (char *)buffer, length);
}

MicroBitAccelerometer::MicroBitAccelerometer(int id, int address) : sample()
{
    // Store our identifiers.
    this->id = id;
    this->address = address;

    // Enable the accelerometer.
    // Firts place the device into standby mode, so it can be configured.
    writeCommand(MMA8652_CTRL_REG1, 0x00);
    
    // Configure for a +/- 2g range.
    writeCommand(MMA8652_XYZ_DATA_CFG, 0x00);
    
    // Bring the device back online, with a 50Hz sample frequency.
    writeCommand(MMA8652_CTRL_REG1, 0x21);
}


/**
  * Reads the acceleration data from the accelerometer, and stores it in our buffer.
  */
void MicroBitAccelerometer::update()
{
    uint8_t data[7];

    readCommand( MMA8652_STATUS, data, 7);
    sample.x = ((data[1] * 256) + ((unsigned short) data[2]));
    sample.y = ((data[3] * 256) + ((unsigned short) data[4]));
    sample.z = ((data[5] * 256) + ((unsigned short) data[6]));
    
    //TODO: Issue an event.
    
};

/**
  * Reads the X axis value of the latest update from the accelerometer.
  * @return The force measured in the X axis, in milli-g.
  */
int MicroBitAccelerometer::getX()
{
    return sample.x;
}

/**
  * Reads the Y axis value of the latest update from the accelerometer.
  * @return The force measured in the Y axis, in milli-g.
  */    
int MicroBitAccelerometer::getY()
{
    return sample.y;
}

/**
  * Reads the Z axis value of the latest update from the accelerometer.
  * @return The force measured in the Z axis, in milli-g.
  */    
int MicroBitAccelerometer::getZ()
{
    return sample.z;
}


