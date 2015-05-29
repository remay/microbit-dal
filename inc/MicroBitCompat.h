/**
  * Compatibility setting for the MicroBit DAL.
  */
  
#ifndef MICROBIT_COMPAT_H
#define MICROBIT_COMPAT_H

#define PI 3.14159265359

 /*
  * Utility functions. 
  *
  * Included here often to reduce the need to import a whole library for simple opertations.
  * This helps to minimize our SRAM footprint.
  */

inline bool min(int a, int b)
{
    return (a < b ? a : b);
}

inline bool max(int a, int b)
{
    return (a > b ? a : b);
}

inline void *memclr(void *a, size_t b)
{
    return memset(a,0,b);
}

#endif