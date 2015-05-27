/**
  * Class definition for MicroBit Compass.
  *
  * Represents an implementation of the Freescale MAG3110 I2C Magnetmometer.
  * Also includes basic caching, calibration and on demand activation.
  */
  
#ifndef MICROBIT_COMPASS_H
#define MICROBIT_COMPASS_H

#include "mbed.h"

/**
  * Relevant pin assignments
  */
#define MICROBIT_PIN_COMPASS_DATA_READY          P0_29

/*
 * MAG3110 Register map
 */
#define MAG_DR_STATUS 0x00
#define MAG_OUT_X_MSB 0x01
#define MAG_OUT_X_LSB 0x02
#define MAG_OUT_Y_MSB 0x03
#define MAG_OUT_Y_LSB 0x04
#define MAG_OUT_Z_MSB 0x05
#define MAG_OUT_Z_LSB 0x06
#define MAG_WHO_AM_I  0x07
#define MAG_SYSMOD    0x08
#define MAG_OFF_X_MSB 0x09
#define MAG_OFF_X_LSB 0x0A
#define MAG_OFF_Y_MSB 0x0B
#define MAG_OFF_Y_LSB 0x0C
#define MAG_OFF_Z_MSB 0x0D
#define MAG_OFF_Z_LSB 0x0E
#define MAG_DIE_TEMP  0x0F
#define MAG_CTRL_REG1 0x10
#define MAG_CTRL_REG2 0x11

/*
 * MAG3110 MAGIC ID value
 * Returned from the MAG_WHO_AM_I register for ID purposes.
 */
#define MAG_3110_WHO_AM_I_VALUE 0xC4

struct CompassSample
{
    int16_t         x;
    int16_t         y;
    int16_t         z;
};

class MicroBitCompass
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
      
    int id;             // Event Bus ID
    int address;        // I2C address of the magnetmometer.  
    
    CompassSample       average;        // Centre point of sample data.
    CompassSample       sample;         // The latest sample data recorded.
    DigitalIn           int1;           // Data ready interrupt.
        
    public:
    
    /**
      * Constructor. 
      * Create a compass representation with the given ID.
      * @param id the ID of the new object.
      */
    MicroBitCompass(int id, int address);
    
    /**
      * Gets the current heading of the device, relative to magnetic north.
      * @return the current heading, in degrees.
      */
    int heading();

    /**
      * Reads the X axis value of the latest update from the compass.
      * @return The magnetic force measured in the X axis, in no specific units.
      */
    int getX();
    
    /**
      * Reads the Y axis value of the latest update from the compass.
      * @return The magnetic force measured in the Y axis, in no specific units.
      */    
    int getY();
    
    /**
      * Reads the Z axis value of the latest update from the compass.
      * @return The magnetic force measured in the Z axis, in no specific units.
      */    
    int getZ();    

    /**
      * periodic callback from MicroBit clock.
      * Check if any data is ready for reading...
      */    
    void tick();
    
    private:
    
    /**
      * Issues a standard, 2 byte I2C command write to the magnetometer.
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
    
    /**
      * Issues a read of a given address, and returns the value.
      * Blocks the calling thread until complete.
      *
      * @param reg The based address of the 16 bit register to access.
      * @return The register value, interpreted as a 16 but signed value.
      */
    int16_t read16(uint8_t reg);
};

#endif
