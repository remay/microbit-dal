/**
  * Class definition for a MicroBitImage.
  *
  * An MicroBitImage is a simple bitmap representation of an image.
  * n.b. This is a mutable, managed type.
  */
  
#ifndef MICROBIT_IMAGE_H
#define MICROBIT_IMAGE_H

#include "mbed.h"

class MicroBitImage
{
    int width;                               // Width of the bitmap, in pixels.
    int height;                              // Height of the bitmap, in pixels.
    int *ref;                                // Reference count.
    
    /**
      * Internal constructor support function. 
      */
    void init(const int x, const int y, const uint8_t *bitmap);
    
    public:
    static MicroBitImage EmptyImage;    // Shared representation of a null image.
    uint8_t *bitmap;                    // 2D array representing the bitmap image.    
    
    /**
      * Default Constructor. 
      * Creates a new reference to the empty MicroBitImage bitmap
      * 
      */
    MicroBitImage();
    
    
  /**
    * Copy Constructor. 
    * Add ourselves as a reference to an existing MicroBitImage.
    * 
    * @param image The MicroBitImage to reference.
    */
    MicroBitImage(const MicroBitImage &image);
    
    /**
      * Constructor. 
      * Create a blank bitmap representation of a given size.
      * 
      * @param s A text based representation of the image given whitespace delimited numeric values.
      */
    MicroBitImage(const char *s);

    /**
      * Constructor. 
      * Create a blank bitmap representation of a given size.
      * 
      * @param x the width of the image.
      * @param y the height of the image.     
      */
    MicroBitImage(const int x, const int y);

    /**
      * Constructor. 
      * Create a bitmap representation of a given size, based on a given buffer.
      * 
      * @param x the width of the image.
      * @param y the height of the image.
      * @param bitmap a 2D array representing the image.
      *     
      */
    MicroBitImage(const int x, const int y, const uint8_t *bitmap);

    /**
      * Destructor. 
      * Removes buffer resources held by the instance.
      */
    ~MicroBitImage();

    /**
      * Copy assign operation. 
      *
      * Called when one MicroBitImage is assigned the value of another using the '=' operator.
      * Decrement our reference count and free up the buffer as necessary.
      * Then, update our buffer to refer to that of the supplied MicroBitImage,
      * and increase its reference count.
      *
      * @param s The MicroBitImage to reference.
      */
    MicroBitImage& operator = (const MicroBitImage& i);


    /**
      * Equality operation.
      *
      * Called when one MicroBitImage is tested to be equal to another using the '==' operator.
      *
      * @param i The MicroBitImage to test ourselves against.
      * @return true if this MicroBitImage is identical to the one supplied, false otherwise.
      */
    bool operator== (const MicroBitImage& i);
    
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
    void setPixelValue(int x , int y, int value);

    /**
      * Determined the value of a given pixel.
      * @return The value assigned to the givne pixel location
      */
    int getPixelValue(int x , int y);

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
    void printImage(int x, int y, const uint8_t *bitmap);
    
    /**
      * Pastes a given bitmap at the given co-ordinates.
      * Any pixels in the relvant area of this image are replaced.
      * 
      * @param image The MicroBitImage to paste.
      * @param x The leftmost X co-ordinate in this image where the given image should be pasted.
      * @param y The uppermost Y co-ordinate in this image where the given image should be pasted.
      * @param alpha set to 1 if transparency clear pixels in given image should be treated as transparent. Set to 0 otherwise.
      * @return The number of pixels written.
      */
    int paste(const MicroBitImage &image, int x, int y, int alpha);
 
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

