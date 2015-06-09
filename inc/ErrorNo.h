#ifndef ERROR_NO_H
#define ERROR_NO_H

//Global error macro
enum Error{
    MICROBIT_INVALID_VALUE = -1, // currently only used in MicroBit.cpp rand function when the max is less or equal to 0.
    MICROBIT_IMG_OOB = -2, // used in MicroBitImage.cpp for when an image x,y is OOB of the current image.
    MICROBIT_IO_OP_NA = -3 // used in MicroBitPin.cpp for when a pin cannot perform a transition. (microbit io operation not allowed)
};
#endif