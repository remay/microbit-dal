#ifndef ERROR_NO_H
#define ERROR_NO_H

//Global error macro
enum Error{
    MICROBIT_INVALID_VALUE = -1, // currently only used in MicroBit.cpp rand function when the max is less or equal to 0.
    MICROBIT_IO_OP_NA = -2, // used in MicroBitPin.cpp for when a pin cannot perform a transition. (microbit io operation not allowed)
    MICROBIT_OOM = 20 // the MicroBit Out of memory error code...
};
#endif