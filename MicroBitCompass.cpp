#include "inc/MicroBit.h"

/**
  * Constructor. 
  * Create a compass representation with the given ID.
  * @param id the event ID of the compass object.
  * @param address the default address for the compass register
  *
  * Example:
  * @code
  * compass(MICROBIT_ID_COMPASS, MAG3110_DEFAULT_ADDR);
  * @endcode
  */
MicroBitCompass::MicroBitCompass(uint16_t id, uint16_t address) : average(), sample(), int1(MICROBIT_PIN_COMPASS_DATA_READY)
{
    this->id = id;
    this->address = address;
    this->calibrating = 0;
    
    // Enable automatic reset after each sample;
    writeCommand(MAG_CTRL_REG2, 0xA0);
    
    // Select 10Hz update rate, with oversampling. Also enables the device.
    writeCommand(MAG_CTRL_REG1, 0x61);    
    
    // Indicate that we're up and running.
    uBit.flags |= MICROBIT_FLAG_COMPASS_RUNNING;
}

/**
  * Issues a standard, 2 byte I2C command write to the magnetometer.
  * Blocks the calling thread until complete.
  *
  * @param reg The address of the register to write to.
  * @param value The value to write.
  */
void MicroBitCompass::writeCommand(uint8_t reg, uint8_t value)
{
    uint8_t command[2];
    command[0] = reg;
    command[1] = value;
    
    uBit.i2c.write(address, (const char *)command, 2);
}

/**
  * Issues a read command into the specified buffer.
  * Blocks the calling thread until complete.
  *
  * @param reg The address of the register to access.
  * @param buffer Memory area to read the data into.
  * @param length The number of bytes to read.
  */
void MicroBitCompass::readCommand(uint8_t reg, uint8_t* buffer, int length)
{
    uBit.i2c.write(address, (const char *)&reg, 1, true);
    uBit.i2c.read(address, (char *)buffer, length);
}


/**
  * Issues a read of a given address, and returns the value.
  * Blocks the calling thread until complete.
  *
  * @param reg The based address of the 16 bit register to access.
  * @return The register value, interpreted as a 16 but signed value.
  */
int16_t MicroBitCompass::read16(uint8_t reg)
{
    uint8_t cmd[2];

    cmd[0] = reg;
    uBit.i2c.write(address, (const char *)cmd, 1);

    cmd[0] = 0x00;
    cmd[1] = 0x00;
    
    uBit.i2c.read(address, (char *)cmd, 2);
    return (int16_t) ((cmd[1] | (cmd[0] << 8))); //concatenate the MSB and LSB
}

/**
  * Issues a read of a given address, and returns the value.
  * Blocks the calling thread until complete.
  *
  * @param reg The based address of the 16 bit register to access.
  * @return The register value, interpreted as a 8 bi signed value.
  */
int16_t MicroBitCompass::read8(uint8_t reg)
{
    int8_t data;

    data = 0;
    readCommand(reg, (uint8_t*) &data, 1);

    return data;
}


/**
  * Gets the current heading of the device, relative to magnetic north.
  * @return the current heading, in degrees.
  *
  * Example:
  * @code
  * uBit.compass.heading();
  * @endcode
  */
int MicroBitCompass::heading()
{
    float bearing = (atan2((double)(sample.y - average.y),(double)(sample.x - average.x)))*180/PI;
    if (bearing < 0)
        bearing += 360.0;
        
    return (int) (360.0 - bearing);
}


/**
  * Periodic callback from MicroBit clock.
  * Check if any data is ready for reading by checking the interrupt.
  */
void MicroBitCompass::tick()
{
    // Poll interrupt line from accelerometer.
    // Active HI. Interrupt is cleared in data read of MAG_OUT_X_MSB.
    if(int1)
    {
        sample.x = read16(MAG_OUT_X_MSB);
        sample.y = read16(MAG_OUT_Y_MSB);
        sample.z = read16(MAG_OUT_Z_MSB);

        if (calibrating)
        {
            minSample.x = min(sample.x, minSample.x);
            minSample.y = min(sample.y, minSample.y);
            minSample.z = min(sample.z, minSample.z);

            maxSample.x = max(sample.x, maxSample.x);
            maxSample.y = max(sample.y, maxSample.y);
            maxSample.z = max(sample.z, maxSample.z);
        }
    }
}

/**
  * Reads the X axis value of the latest update from the compass.
  * @return The magnetic force measured in the X axis, in no specific units.
  *
  * Example:
  * @code
  * uBit.compass.getX();
  * @endcode
  */
int MicroBitCompass::getX()
{
    return sample.x;
}

/**
  * Reads the Y axis value of the latest update from the compass.
  * @return The magnetic force measured in the Y axis, in no specific units.
  *
  * Example:
  * @code
  * uBit.compass.getY();
  * @endcode
  */     
int MicroBitCompass::getY()
{
    return sample.y;
}

/**
  * Reads the Z axis value of the latest update from the compass.
  * @return The magnetic force measured in the Z axis, in no specific units.
  *
  * Example:
  * @code
  * uBit.compass.getZ();
  * @endcode
  */     
int MicroBitCompass::getZ()
{
    return sample.z;
}

/**
  * Attempts to determine the 8 bit ID from the magnetometer. 
  * @return the id of the compass (magnetometer)
  *
  * Example:
  * @code
  * uBit.compass.whoAmI();
  * @endcode
  */
int MicroBitCompass::whoAmI()
{
    uint8_t data;

    readCommand(MAG_WHOAMI, &data, 1);    
    return (int)data;
}

/**
  * Perform a calibration of the compass.
  * **THIS MUST BE CALLED TO GAIN RELIABLE VALUES FROM THE COMPASS**
  */
void MicroBitCompass::calibrateStart()
{
    calibrating = 1;

    // Take a sane snapshot to start with.
    minSample = sample;
    maxSample = sample;
}    

/**
  * Complete the calibration of the compass.
  * **THIS MUST BE CALLED TO GAIN RELIABLE VALUES FROM THE COMPASS**
  */ 
void MicroBitCompass::calibrateEnd()
{
    calibrating = 0;
    
    average.x = (maxSample.x + minSample.x) / 2;
    average.y = (maxSample.y + minSample.y) / 2;
    average.z = (maxSample.z + minSample.z) / 2;
}    

/**
  * Returns 0 or 1. 1 indicates data is waiting to be read, zero means data is not ready to be read.
  */
int MicroBitCompass::isDataReady()
{
    //Active HI
    return int1;
}