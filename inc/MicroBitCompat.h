/**
  * Compatibility / portability funcitons and constants for the MicroBit DAL.
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

inline int min(int a, int b)
{
    return (a < b ? a : b);
}

inline int max(int a, int b)
{
    return (a > b ? a : b);
}

inline void *memclr(void *a, size_t b)
{
    return memset(a,0,b);
}

inline bool isdigit(char c)
{
    return (c > 47 && c < 58);
}

/**
  * Performs an in buffer reverse of a given char array
  */
void string_reverse(char *s);

/**
  * Converts a given integer into a base 10 ASCII equivalent.
  *
  * @param n The number to convert.
  * @param s Pointer to a buffer in which to store the resulting string.
  */
void itoa(int n, char *s);


#endif