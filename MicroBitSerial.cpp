#include "MicroBit.h"
#include "mbed.h"

/**
  * Constructor. 
  * Create an instance of MicroBitSerial
  * @param tx the Pin to be used for transmission
  * @param rx the Pin to be used for receiving data
  * Example:
  * @code 
  * MicroBitSerial serial(USBTX, USBRX);
  * @endcode
  * @note the default baud rate is 115200
  */
MicroBitSerial::MicroBitSerial(PinName tx, PinName rx) : Serial(tx,rx)
{
    this->baud(MICROBIT_SERIAL_DEFAULT_BAUD_RATE);
    this->buffer[MICROBIT_SERIAL_BUFFER_SIZE-1] = '\0' ;
}

void MicroBitSerial::printString(ManagedString s)
{   
    const int len = s.length();
    const char *data = s.toCharArray();
    
    Serial::write(data,len);   
}

ManagedString MicroBitSerial::readString()
{   
    //read up until the null terminator
    
    //needs work, read until we timeout
    //custom implementation...?
    Serial::read(buffer,MICROBIT_SERIAL_BUFFER_SIZE-2); 
    
    return ManagedString(buffer);   
}

