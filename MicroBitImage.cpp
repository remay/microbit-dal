/**
  * Class definition for a MicroBitImage.
  *
  * An MicroBitImage is a simple bitmap representation of an image.
  */

#include "MicroBitImage.h"
#include "MicroBitCompat.h"

#define MICROBIT_FONT_WIDTH 4
#define MICROBIT_FONT_HEIGHT 5
#define MICROBIT_FONT_ASCII_START 32
#define MICROBIT_FONT_ASCII_END 126



/*
 * The original MicroBug font, courtesy of Michael Sparks esq.
 * The font is 4x5, so we compress in into a linear nibble array to conserve memory.
 *
 * Format: {0xR1:R2, 0xR3:R4, 0xR5:0....}
 *
 * We could compress further, but the complexity of decode would likely outweigh the gains.
 */
const unsigned char font[] = {0x0, 0x0, 0x0, 0x44, 0x40, 0x40, 0xaa, 0x0, 0x0, 0xaf, 0xaf, 0xa0, 0x6d, 0x6b, 0x60, 0x92, 0x49, 0x0, 0x64, 0x96, 0x0, 0x88, 0x0, 0x0, 0x12, 0x22, 0x10, 0x84, 0x44, 0x80, 0xa, 0x4a, 0x0, 0x4, 0xe4, 0x0, 0x0, 0x4, 0x40, 0x0, 0xe0, 0x0, 0x0, 0x4, 0x0, 0x22, 0x48, 0x80, 0x4a, 0xaa, 0x40, 0x4c, 0x44, 0x40, 0xe1, 0x68, 0xf0, 0xf1, 0x29, 0x60, 0x8a, 0xaf, 0x20, 0xf8, 0x61, 0xe0, 0x78, 0xe9, 0xe0, 0xf1, 0x24, 0x40, 0x69, 0x69, 0x60, 0x79, 0x71, 0x10, 0x4, 0x4, 0x0, 0x4, 0x4, 0x40, 0x12, 0x42, 0x10, 0xe, 0xe, 0x0, 0x42, 0x12, 0x40, 0x69, 0x20, 0x60, 0x7b, 0xb8, 0x60, 0x69, 0x9f, 0x90, 0xe9, 0xe9, 0xe0, 0x78, 0x88, 0x70, 0xe9, 0x99, 0xe0, 0xf8, 0xe8, 0xf0, 0xf8, 0xe8, 0x80, 0x78, 0xb9, 0x70, 0x99, 0xf9, 0x90, 0xe4, 0x44, 0xe0, 0xf1, 0x19, 0x60, 0x9a, 0xca, 0x90, 0x88, 0x88, 0xf0, 0x9f, 0xff, 0x90, 0x9d, 0xbb, 0x90, 0x69, 0x99, 0x60, 0xe9, 0xe8, 0x80, 0x69, 0x96, 0x30, 0xe9, 0xe9, 0x90, 0x78, 0x61, 0xe0, 0x72, 0x22, 0x20, 0x99, 0x99, 0x60, 0x99, 0x9a, 0x40, 0x9b, 0xff, 0x90, 0x99, 0x69, 0x90, 0x95, 0x24, 0x80, 0xf2, 0x48, 0xf0, 0x32, 0x22, 0x30, 0x44, 0x21, 0x10, 0xc4, 0x44, 0xc0, 0x4a, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x42, 0x0, 0x0, 0x7, 0x97, 0x0, 0x8e, 0x9e, 0x0, 0x7, 0x87, 0x0, 0x17, 0x97, 0x0, 0x69, 0xe7, 0x0, 0x34, 0xe4, 0x0, 0xf, 0x9f, 0xf0, 0x8e, 0x99, 0x0, 0x60, 0x66, 0x0, 0x20, 0x44, 0x80, 0x8b, 0xe9, 0x0, 0x44, 0x46, 0x0, 0xe, 0xbb, 0x0, 0xc, 0xaa, 0x0, 0x6, 0x96, 0x0, 0xe, 0x9e, 0x80, 0x7, 0x97, 0x10, 0x3, 0x44, 0x0, 0x3, 0x6e, 0x0, 0x46, 0x42, 0x0, 0x9, 0x97, 0x0, 0x9, 0xa4, 0x0, 0x9, 0xb6, 0x0, 0x9, 0x69, 0x0, 0x9, 0xd2, 0xc0, 0xf, 0x4f, 0x0, 0x32, 0x62, 0x30, 0x44, 0x44, 0x40, 0xc4, 0x64, 0xc0, 0x5a, 0x0, 0x0};

/**
  * Constructor. 
  * Create a blank bitmap representation of a given size.
  * 
  * @param x the width of the image.
  * @param y the height of the image.     
  */
MicroBitImage::MicroBitImage(int x, int y)
{
    this->init(x,y,NULL);

}

/**
  * Constructor. 
  * Create a bitmap representation of a given size.
  * 
  * @param x the width of the image.
  * @param y the height of the image.
  * @param bitmap a 2D array representing the image.
  *     
  */
MicroBitImage::MicroBitImage(int x, int y, int **bitmap)
{
    this->init(x,y,bitmap);
}

MicroBitImage::~MicroBitImage()
{
    for (int i = 0; i < height; i++)
            delete[] this->bitmap[i];

    delete[] this->bitmap;
}

void MicroBitImage::init(int x, int y, int **bitmap)
{
    // Create a copy of the array
    this->width = x;
    this->height = y;
    
    // create a jagged array to represent the image. We use a jagged rather than 2D array here to 
    // ease type checking across the myriad of languages that might target us.
    this->bitmap = new int*[width];

    for (int i = 0; i < width; i++)
        this->bitmap[i] = new int[height];

    if (bitmap)
        this->printImage(0,0,bitmap);
    else
        this->clear();
}



/**
  * Clears all pixels in this image
  */
void MicroBitImage::clear()
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            this->bitmap[x][y] = 0;
}
 


/**
  * Sets the pixel at the given co-ordinates to a given value.
  * @param x The co-ordinate of the pixel to change w.r.t. top left origin.
  * @param y The co-ordinate of the pixel to change w.r.t. top left origin.
  * @param value The new value of the pixel.
  */
void MicroBitImage::setPixelValue(int x , int y, int value)
{
    this->bitmap[x][y] = value;
}

/**
  * Determined the value of a given pixel.
  * @return The value assigned to the givne pixel location
  */
int MicroBitImage::getPixelValue(int x , int y)
{
    return this->bitmap[x][y];
}

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
void MicroBitImage::printImage(int x, int y, int **bitmap)
{
    for (int i = 0; i < min(x,width); i++)
        for (int j = 0; j < min(y,height); j++)
            this->bitmap[i][j] = bitmap[i][j];
}
  
/**
  * Pastes a given bitmap at the given co-ordinates.
  * Any pixels in the relvant area of this image are replaced.
  * 
  * @param image The MicroBitImage to paste.
  * @param x The leftmost X co-ordinate in this image where the given image should be pasted.
  * @param y The uppermost Y co-ordinate in this image where the given image should be pasted.
  * @param alpha set to 1 if transparency clear pixels in given image should be treated as transparent. Set to 0 otherwise.
  */
void MicroBitImage::paste(MicroBitImage *image, int x, int y, int alpha)
{
    // Sanity check.
    if (x >= width || y >= height)
        return;
    
    // Paste.
    for (int i = 0; i < min(image->width,width-x); i++)
        for (int j = 0; j < min(image->height,height-y); j++)
            if (!(alpha && bitmap[i][j] == 0)) 
                this->bitmap[i+x][j+y] = bitmap[i][j];        
}

 /**
  * Prints a character to the display at the given location
  *
  * @param c The character to display.
  * @param x The x co-ordinate of on the image to place the top left of the character
  * @param y The y co-ordinate of on the image to place the top left of the character
  */
void MicroBitImage::print(char c, int x, int y)
{
    
    unsigned char v;
    
    // Sanity check. Silently ignore anything out of bounds.
    if (x >= width || y >= height || c < MICROBIT_FONT_ASCII_START || c > MICROBIT_FONT_ASCII_END)
        return;
    
    // Paste.
    int offset = (c-MICROBIT_FONT_ASCII_START) * 3;
    
    for (int row=0; row<MICROBIT_FONT_HEIGHT; row++)
    {
        v = font[offset];
        
        if (row%2 == 0)
            v = v >> 4;
        else
            offset++;
        
        for (int col = 0; col < MICROBIT_FONT_WIDTH; col++)
            if (x+col < width && y+row < height)
                this->bitmap[x+col][y+row] = (v & (0x08 >> col)) ? 1 : 0;
        
    }  
}


/**
  * Shifts the pixels in this Image a given number of pixels to the Left.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftLeft(int n)
{
    for (int i = 0; i < width-1 ; i++)
        for (int j = 0; j < height; j++)
                this->bitmap[i][j] = this->bitmap[i+1][j];         
                
    // Blank fill the rightmost column.
    for (int j = 0; j < height; j++)
        this->bitmap[width-1][j] = 0;
        
}

/**
  * Shifts the pixels in this Image a given number of pixels to the Right.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftRight(int n)
{
    for (int i = 0; i < width-1 ; i++)
        for (int j = 0; j < height; j++)
                this->bitmap[i+1][j] = this->bitmap[i][j];         

    // Blank fill the leftmost column.
    for (int j = 0; j < height; j++)
        this->bitmap[0][j] = 0;
}


/**
  * Shifts the pixels in this Image a given number of pixels to Upward.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftUp(int n)
{
    for (int i = 0; i < width ; i++)
        for (int j = 0; j < height-1; j++)
            this->bitmap[i][j] = this->bitmap[i][j+1];         
            
    // Blank fill the bottommost row.
    for (int i = 0; i < width; i++)
        this->bitmap[i][height] = 0;
}


/**
  * Shifts the pixels in this Image a given number of pixels to Downward.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftDown(int n)
{
    for (int i = 0; i < width ; i++)
        for (int j = 0; j < height-1; j++)
            this->bitmap[i][j+1] = this->bitmap[i][j];         

    // Blank fill the topmost row.
    for (int i = 0; i < height; i++)
        this->bitmap[i][0] = 0;

}
/**
  * Gets the width of this image.
  *
  * @return The width of this image.
  */
int MicroBitImage::getWidth()
{
    return width;
}   

/**
  * Gets the height of this image.
  *
  * @return The height of this image.
  */
int MicroBitImage::getHeight()
{
    return height;
}   
