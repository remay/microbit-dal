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
}

void MicroBitSerial::printString(ManagedString s)
{   
    const int len = s.length();
    const char *data = s.toCharArray();
    
    Serial::write(data,len);   
}

ManagedString MicroBitSerial::readString(int len)
{   
    if(len < 3)
        len = 3;

    char buffer[len];
    
    memset(buffer, 0, sizeof(buffer));
    
    //needs work, read until we timeout
    //custom implementation...?
    int length = 0;
    uBit.display.scrollString((int)'\n');
    length = readChars(buffer,len, '\n'); 
    
    if(length == 0)
        return ManagedString();
    
    //add in a null terminator so bad things don't happen with ManagedString
    buffer[length] = '\0';
    
    return ManagedString(buffer);   
}

void MicroBitSerial::printImage(MicroBitImage i)
{
       
}

MicroBitImage MicroBitSerial::readImage()
{
    return MicroBitImage();
}

void MicroBitSerial::screenshot()
{
    
}

ssize_t MicroBitSerial::readChars(void* buffer, size_t length, char eof) {
    
    char* ptr = (char*)buffer;
    char* end = ptr + length;
    
    int eofAscii = (int)eof;
    
    while (ptr != end) {
        
        int c = _getc();
        
        //check EOF
        if (c == eofAscii) 
            break;
        
        //store the character    
        *ptr++ = c;    
    }      
    
    return ptr - (const char*)buffer;
}


