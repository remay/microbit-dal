/**
  * Class definition for a MicroBitImage.
  *
  * An MicroBitImage is a simple bitmap representation of an image.
  */
  
#ifndef MICROBIT_IMAGE_H
#define MICROBIT_IMAGE_H

#include "mbed.h"

class MicroBitImage
{
    int width;              // Width of the bitmap, in pixels.
    int height;             // Height of the bitmap, in pixels.
    
    /**
      * Internal constructor support function. 
      */
    void init(int x, int y, uint8_t *bitmap);
    
    public:
    uint8_t *bitmap;        // 2D array representing the bitmap image.    
    
    /**
      * Copy Constructor. 
      * Clone an existing MicroBitImage.
      * 
      * @param image The MicroBitImage to clone.
      */
    MicroBitImage(MicroBitImage &image);
    
    /**
      * Constructor. 
      * Create a blank bitmap representation of a given size.
      * 
      * @param x the width of the image.
      * @param y the height of the image.     
      */
    MicroBitImage(int x, int y);

    /**
      * Constructor. 
      * Create a bitmap representation of a given size.
      * 
      * @param x the width of the image.
      * @param y the height of the image.
      * @param bitmap a 2D array representing the image.
      *     
      */
    MicroBitImage(int x, int y, uint8_t *bitmap);

    /**
      * Destructor. 
      * Removes buffer resources held by the instance.
      */
    ~MicroBitImage();


    /**
      * Clears all pixels in this image
      */
    void clear();

    /**
      * Sets the pixel at the given co-ordinates to a given value.
      * @param x The co-ordinate of the pixel to change w.r.t. top left origin.
      * @param y The co-ordinate of the pixel to change w.r.t. top left origin.
      * @param value The new value of the pixel.
      */
    void setPixelValue(int x , int y, uint8_t value);

    /**
      * Determined the value of a given pixel.
      * @return The value assigned to the givne pixel location
      */
    uint8_t getPixelValue(int x , int y);

    /**
      * Replaces the content of this image with that of a given 
      * 2D array representing the image.
      * Origin is in the top left corner of the image.
      *
      * @param x the width of the image.
      * @param y the height of the image.
      * @param bitmap a 2D array representing the image.
      *     
      */
    void printImage(int x, int y, uint8_t *bitmap);
    
    /**
      * Pastes a given bitmap at the given co-ordinates.
      * Any pixels in the relvant area of this image are replaced.
      * 
      * @param image The MicroBiImage to paste.
      * @param x The leftmost X co-ordinate in this image where the given image should be pasted.
      * @param y The uppermost Y co-ordinate in this image where the given image should be pasted.
      * @param alpha set to 1 if transparency clear pixels in given image should be treated as transparent. Set to 0 otherwise.
      */
    void paste(MicroBitImage &image, int x, int y, int alpha);
 
     /**
      * Prints a character to the display at the given location
      *
      * @param c The character to display.
      * @param x The x co-ordinate of on the image to place the top left of the character
      * @param y The y co-ordinate of on the image to place the top left of the character
      */
    void print(char c, int x, int y);
 
    /**
      * Shifts the pixels in this Image a given number of pixels to the Left.
      *
      * @param n The number of pixels to shift.
      */
    void shiftLeft(int n);

    /**
      * Shifts the pixels in this Image a given number of pixels to the Right.
      *
      * @param n The number of pixels to shift.
      */
    void shiftRight(int n);
    
    /**
      * Shifts the pixels in this Image a given number of pixels to Upward.
      *
      * @param n The number of pixels to shift.
      */
    void shiftUp(int n);
    
    /**
      * Shifts the pixels in this Image a given number of pixels to Downward.
      *
      * @param n The number of pixels to shift.
      */
    void shiftDown(int n);

    /**
      * Gets the width of this image.
      *
      * @return The width of this image.
      */
    int getWidth();

    /**
      * Gets the height of this image.
      *
      * @return The height of this image.
      */
    int getHeight();
};

#endif

