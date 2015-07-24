#include "MicroBit.h"
#include "mbed.h"
#include "twi_master.h"

/**
  * Constructor. 
  * Create an instance of i2c
  * @param sda the Pin to be used for SDA
  * @param scl the Pin to be used for SCL
  * Example:
  * @code 
  * MicroBitI2C i2c(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL);
  * @endcode
  * @note this should prevent i2c lockups as well.
  */
MicroBitI2C::MicroBitI2C(PinName sda, PinName scl) : I2C(sda,scl)
{
    this->retries = 0;
}


int MicroBitI2C::read(int address, char *data, int length, bool repeated)
{
    int result = I2C::read(address,data,length,repeated);
    
    while(result == I2C_ERROR_BUS_BUSY && retries < MICROBIT_I2C_MAX_RETRIES)
    {
        //waiting for the platform to be available
        twi_master_init_and_clear();
        result = I2C::read(address,data,length,repeated);
        retries++;
    }
    
    if(retries == MICROBIT_I2C_MAX_RETRIES - 1)
        uBit.panic(MICROBIT_I2C_LOCKUP);
    
    retries = 0;
    
    return result;
}

int MicroBitI2C::write(int address, const char *data, int length, bool repeated)
{   
    int result = I2C::write(address,data,length,repeated);
    
    while(result == I2C_ERROR_BUS_BUSY && retries < MICROBIT_I2C_MAX_RETRIES)
    {
        twi_master_init_and_clear();
        result = I2C::write(address,data,length,repeated);
        retries++;
    }
    
    if(retries == MICROBIT_I2C_MAX_RETRIES - 1)
        uBit.panic(MICROBIT_I2C_LOCKUP);
        
    retries = 0;
    
    return result;
}
