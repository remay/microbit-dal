/**
  * Class definition for MicroBit Magnetometer.
  *
  * Represents an implementation of the Freescale MAG3110 I2C Magnetmometer.
  * Also includes basic caching, calibration and on demand activation.
  */
  
#ifndef MICROBIT_MAGNETOMETER_H
#define MICROBIT_MAGNETOMETER_H

#include "mbed.h"

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


/*
 * MAG3110 Sample Rate constants
 */
#define MAG_3110_SAMPLE80 0
#define MAG_3110_SAMPLE40 0x20
#define MAG_3110_SAMPLE20 0x40
#define MAG_3110_SAMPLE10 0x60
#define MAG_3110_SAMPLE5 0x80
#define MAG_3110_SAMPLE2_5 0xA0
#define MAG_3110_SAMPLE1_25 0xC0
#define MAG_3110_SAMPLE0_625 0xE0

#define MAG_3110_OVERSAMPLE1 0
#define MAG_3110_OVERSAMPLE2 0x08
#define MAG_3110_OVERSAMPLE3 0x10
#define MAG_3110_OVERSAMPLE4 0x18

// read only 1 byte per axis
#define MAG_3110_FASTREAD 0x04
// do one measurement (even if in standby mode)
#define MAG_3110_TRIGGER 0x02
// put in active mode
#define MAG_3110_ACTIVE 0x01

// CTRL_REG2: AUTO_MRST_EN  _ RAW MAG_RST _ _ _ _ _
// reset sensor after each reading
#define MAG_3110_AUTO_MRST_EN 0x80
// don't subtract user offsets
#define MAG_3110_RAW 0x20
// reset magnetic sensor after too-large field
#define MAG_3110_MAG_RST 0x10

// DR_STATUS Register ZYXOW ZOW YOW XOW ZYXDR ZDR YDR XDR
#define MAG_3110_ZYXDR  0x08



class MicroBitMagnetometer
{
    /**
      * Unique, enumerated ID for this component. 
      * Used to track asynchronous events in the event bus.
      */
      
    int id;             // Event Bus ID
    int address;        // I2C address of the magnetmometer.  
    
    public:
    
    /**
      * Constructor. 
      * Create a magnetometer representation with the given ID.
      * @param id the ID of the new object.
      */
    MicroBitMagnetometer(int id, int address);
    
    /**
      * Gets the current heading of the device, relative to magnetic north.
      * @return the current heading, in degrees.
      */
    int heading();
    
};

#endif
