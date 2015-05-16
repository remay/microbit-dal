/**
  * Compatibility setting for the MicroBit DAL.
  */
  
#ifndef MICROBIT_COMPAT_H
#define MICROBIT_COMPAT_H

#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)
#define memclr(a,b) (memset(a,0,b))

#define PI 3.14159265359


#endif