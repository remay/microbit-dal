#ifndef MICROBIT_MALLOC_H
#define MICROBIT_MALLOC_H

#include "MicroBit.h"
#include <new> 

/**
  * Overrides malloc globally, and fires the panic function if we run out of memory!
  */
inline void* ubit_malloc(size_t size)
{
    void *ptr;
 
    ptr = malloc(size);
    
    if(ptr == NULL)
        panic(MICROBIT_OOM);
    
    return ptr;
}

/**
  * Overrides malloc globally, and fires the panic function if we run out of memory!
  * could just call ubit_malloc???
  */
inline void* operator new(size_t size) throw(std::bad_alloc)
{   
    void *ptr;
 
    ptr = malloc(size);
    
    if(ptr == NULL)
        panic(MICROBIT_OOM);
    
    return ptr;
}

#define malloc(X) ubit_malloc( X )

#endif
