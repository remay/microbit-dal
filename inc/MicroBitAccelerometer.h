/**
  * Class definition for MicroBit Accelerometer.
  *
  * Represents an implementation of the Freescale MMA8652 3 axis accelerometer
  * Also includes basic data caching and on demand activation.
  */
  
#ifndef MICROBIT_ACCELEROMETER_H
#define MICROBIT_ACCELEROMETER_H

#include "mbed.h"

/*
 * I2C constants
 */
#define MMA8652_DEFAULT_ADDR    0x3A

/*
 * MMA8652 Register map
 */
#define MMA8652_STATUS          0x00
#define MMA8652_OUT_X_MSB       0x01
#define MMA8652_WHOAMI          0x0D
#define MMA8652_XYZ_DATA_CFG    0x0E
#define MMA8652_CTRL_REG1       0x2A
#define MMA8652_WHOAMI_VAL      0x4A

struct MMA8652Sample
{
    int16_t         x;
    int16_t         y;
    int16_t         z;
};

class MicroBitAccelerometer
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
      
    int             id;            // Event Bus ID
    int             address;       // I2C address of this accelerometer.
    MMA8652Sample   sample;        // The last sample read.

    public:
    
    /**
      * Constructor. 
      * Create an accelerometer representation with the given ID.
      * @param id the ID of the new object.
      */
    MicroBitAccelerometer(int id, int address);
    
    /**
      * Reads the acceleration data from the accelerometer, and stores it in our buffer.
      */
    void update();

    /**
      * Reads the X axis value of the latest update from the accelerometer.
      * @return The force measured in the X axis, in milli-g.
      */
    int getX();
    
    /**
      * Reads the Y axis value of the latest update from the accelerometer.
      * @return The force measured in the Y axis, in milli-g.
      */    
    int getY();
    
    /**
      * Reads the Z axis value of the latest update from the accelerometer.
      * @return The force measured in the Z axis, in milli-g.
      */    
    int getZ();

    private:
    /**
      * Issues a standard, 2 byte I2C command write to the accelerometer.
      * Blocks the calling thread until complete.
      *
      * @param reg The address of the register to write to.
      * @param value The value to write.
      */
    void writeCommand(uint8_t reg, uint8_t value);
    
    /**
      * Issues a read command into the specified buffer.
      * Blocks the calling thread until complete.
      *
      * @param reg The address of the register to access.
      * @param buffer Memory area to read the data into.
      * @param length The number of bytes to read.
      */
    void readCommand(uint8_t reg, uint8_t* buffer, int length);

};

#endif
