#include "MicroBit.h"
#ifndef MICROBIT_SERIAL_H
#define MICROBIT_SERIAL_H

#include "MicroBitComponent.h"
#include "mbed.h"

#define MICROBIT_SERIAL_DEFAULT_BAUD_RATE 115200
#define MICROBIT_SERIAL_BUFFER_SIZE 20

#define MICROBIT_SERIAL_DEFAULT_EOF '\n'

/**
  * Class definition for MicroBitSerial.
  *
  * Represents an instance of Serial which accepts micro:bit specific data types.
  */
class MicroBitSerial : public Serial
{
    ssize_t readChars(void* buffer, size_t length, char eof = MICROBIT_SERIAL_DEFAULT_EOF);
    
    public:
    
    /**
      * Constructor. 
      * Create an instance of MicroBitSerial
      * @param sda the Pin to be used for SDA
      * @param scl the Pin to be used for SCL
      * Example:
      * @code 
      * MicroBitSerial serial(USBTX, USBRX);
      * @endcode
      */
    MicroBitSerial(PinName tx, PinName rx);
    
    void printString(ManagedString s);
    
    ManagedString readString(int len = MICROBIT_SERIAL_BUFFER_SIZE);
    
    void printImage(MicroBitImage i);

    MicroBitImage readImage();
    
    void screenshot();
    
};

#endif
