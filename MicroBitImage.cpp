/**
  * Class definition for a MicroBitImage.
  *
  * An MicroBitImage is a simple bitmap representation of an image.
  * n.b. This is a mutable, managed type.
  */

#include "MicroBit.h"

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

/*
 * The null image. We actally create a small one byte buffer here, just to keep NULL pointers out of the equation.
 */
MicroBitImage MicroBitImage::EmptyImage(1,1);

/**
  * Default Constructor. 
  * Creates a new reference to the empty MicroBitImage.
  * 
  */
MicroBitImage::MicroBitImage()
{
    // Create new reference to the EmptyImage and we're done.
    *this = MicroBitImage::EmptyImage;
}


/**
  * Constructor. 
  * Create a blank bitmap representation of a given size.
  * 
  * @param x the width of the image.
  * @param y the height of the image.     
  *
  * Bitmap buffer is linear, with 8 bits per pixel, row by row, 
  * top to bottom with no word alignment. Stride is therefore the image width in pixels.
  * in where w and h are width and height respectively, the layout is therefore:
  *
  * |[0,0]...[w,o][1,0]...[w,1]  ...  [[w,h]
  *
  * A copy of the image is made in RAM, as images are mutable.
  *
  * TODO: Consider an immutable flavour, which might save us RAM for animation spritesheets...
  * ...as these could be kept in FLASH.
  */
MicroBitImage::MicroBitImage(const int x, const int y)
{
    this->init(x,y,NULL);
}

/**
    * Copy Constructor. 
    * Add ourselves as a reference to an existing MicroBitImage.
    * 
    * @param image The MicroBitImage to reference.
    */
MicroBitImage::MicroBitImage(const MicroBitImage &image)
{
    bitmap = image.bitmap;
    width = image.width;
    height = image.height;
    ref = image.ref;

    (*ref)++;

    if (*ref == 1)
    {
        (*ref)++;
        delete &image;
    }    
}

/**
  * Constructor. 
  * Create a blank bitmap representation of a given size.
  * 
  * @param s A text based representation of the image given whitespace delimited numeric values.
  */
MicroBitImage::MicroBitImage(const char *s)
{
    int width = 0;
    int height = 0;
    int count = 0;
    int digit = 0;

    char parseBuf[10];

    const char *parseReadPtr;
    char *parseWritePtr;
    uint8_t *bitmapPtr;

    if (s == NULL)
        return;

    // First pass: Parse the string to determine the geometry of the image.
    // We do this from first principles to avoid unecessary load of the strtok() libs etc.
    parseReadPtr = s;

    while (*parseReadPtr)
    {
        if (isdigit(*parseReadPtr))
        {
            // Ignore numbers.
            digit = 1;
        } 
        else if (*parseReadPtr =='\n')
        {
            if (digit)
            {
                count++;
                digit = 0;
            }

            height++;

            width = count > width ? count : width;
            count = 0;
        } 
        else
        {
            if (digit)
            {
                count++;
                digit = 0;
            }
        }

        parseReadPtr++;
    }

    // Store the geomtery.
    this->width = width;
    this->height = height;
    this->bitmap = new uint8_t[width * height];


    // Second pass: collect the data.
    parseReadPtr = s;
    parseWritePtr = parseBuf;
    bitmapPtr = this->bitmap;

    while (*parseReadPtr)
    {
        if (isdigit(*parseReadPtr))
        {
            *parseWritePtr = *parseReadPtr;     
            parseWritePtr++;
        } 
        else
        {
            *parseWritePtr = 0;
            if (parseWritePtr > parseBuf)
            {
                *bitmapPtr = atoi(parseBuf);
                bitmapPtr++;
                parseWritePtr = parseBuf;
            }
        }

        parseReadPtr++;
    }
}

/**
  * Constructor. 
  * Create a new bitmap representation of a given size.
  * 
  * @param x the width of the image.
  * @param y the height of the image.
  * @param the bitmap buffer to copy. 
  *
  */
MicroBitImage::MicroBitImage(const int x, const int y, const uint8_t *bitmap)
{
    this->init(x,y,bitmap);
}

MicroBitImage::~MicroBitImage()
{
    if(--(*ref) == 0)
    {
        delete[] bitmap;
        delete ref;
    }
}

void MicroBitImage::init(const int x, const int y, const uint8_t *bitmap)
{
    // Create a copy of the array
    this->width = x;
    this->height = y;
    
    // create a linear buffer to represent the image. We could use a jagged/2D array here, but experimentation
    // showed this had a negative effect on memory management (heap fragmentation etc).
     
    this->bitmap = new uint8_t[width*height];

    if (bitmap)
        this->printImage(x,y,bitmap);
    else
        this->clear();
        
    ref = (int *) malloc(sizeof(int));
    *ref = 1;
}



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
MicroBitImage& MicroBitImage::operator = (const MicroBitImage& i)
{
    if(this == &i)
        return *this;
     
    bitmap = i.bitmap;
    width = i.width;
    height = i.height;
    ref = i.ref;
    
    (*ref)++;

    // Handle the case where our operand is actually generated soley for us to consume...
    // Typically this is because it's been created by this class in a service function.
    if (*ref == 1)
    {
        (*ref)++;
        delete &i;
    }

    return *this;
}

/**
  * Equality operation.
  *
  * Called when one MicroBitImage is tested to be equal to another using the '==' operator.
  *
  * @param i The MicroBitImage to test ourselves against.
  * @return true if this MicroBitImage is identical to the one supplied, false otherwise.
  */
bool MicroBitImage::operator== (const MicroBitImage& i)
{
    if (bitmap == i.bitmap)
        return true;
    else
        return ((width == i.width) && (height == i.height) && (memcmp(bitmap, i.bitmap,width*height)==0));    
}


/**
  * Clears all pixels in this image
  */
void MicroBitImage::clear()
{
    memclr(this->bitmap, width*height);
}
 
/**
  * Sets the pixel at the given co-ordinates to a given value.
  * @param x The co-ordinate of the pixel to change w.r.t. top left origin.
  * @param y The co-ordinate of the pixel to change w.r.t. top left origin.
  * @param value The new value of the pixel.
  */
void MicroBitImage::setPixelValue(int x , int y, uint8_t value)
{
    this->bitmap[y*width+x] = value;
}

/**
  * Determined the value of a given pixel.
  * @return The value assigned to the givne pixel location
  */
uint8_t MicroBitImage::getPixelValue(int x , int y)
{
    return this->bitmap[y*width+x];
}

/**
  * Replaces the content of this image with that of a given 
  * bitmap representation of an image.
  *
  * Data is copied. Any out of range data is safely ignored.
  *
  * @param x the width of the image.
  * @param y the height of the image.
  * @param linear bitmap representing the image.
  *     
  */
void MicroBitImage::printImage(int width, int height, const uint8_t *bitmap)
{
    const uint8_t *pIn;
    uint8_t *pOut;
    int pixelsToCopyX, pixelsToCopyY;

    // Sanity check.
    if (width <= 0 || width <= 0)
        return;

    // Calcualte sane start pointer.
    pixelsToCopyX = min(width,this->width);
    pixelsToCopyY = min(height,this->height);

    pIn = bitmap;
    pOut = this->bitmap;
    
    // Copy the image, stride by stride.
    for (int i=0; i<pixelsToCopyY; i++)
    {
        memcpy(pOut, pIn, pixelsToCopyX);
        pIn += width;
        pOut += this->width;
    }
}
  
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
int MicroBitImage::paste(MicroBitImage &image, int x, int y, int alpha)
{
    uint8_t *pIn, *pOut;
    int cx, cy;
    int pxWritten = 0;

    // Sanity check.
    // We permit writes that overlap us, but ones that are clearly out of scope we can filter early.
    if (x >= width || y >= height || x+image.width <= 0 || y+image.height <= 0)
        return 0;

    //Calculate the number of byte we need to copy in each dimension.
    cx = x < 0 ? min(image.width + x, width) : min(image.width, width - x);
    cy = y < 0 ? min(image.height + y, height) : min(image.height, height - y);

    // Calcualte sane start pointer.
    pIn = image.bitmap;
    pIn += (x < 0) ? -x : 0;
    pIn += (y < 0) ? -image.width*y : 0;
    
    pOut = bitmap;
    pOut += (x > 0) ? x : 0;
    pOut += (y > 0) ? width*y : 0;

    // Copy the image, stride by stride
    // If we want primitive transparecy, we do this byte by byte.
    // If we don't, use a more efficient block memory copy instead. Every little helps!

    if (alpha)
    {
        for (int i=0; i<cy; i++)
        {
            for (int j=0; j<cx; j++)
            {
                // Copy this byte if appropriate.
                if (*(pIn+j) != 0){
                    *(pOut+j) = *(pIn+j);
                    pxWritten++;
                }
            }
    
            pIn += image.width;
            pOut += width;
        }
    }
    else
    {
        for (int i=0; i<cy; i++)
        {
            memcpy(pOut, pIn, cx);

            pxWritten += cx;
            pIn += image.width;
            pOut += width;
        }
    }
    
    return pxWritten;
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
    int x1, y1;
    
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
        
        // Update our Y co-ord write position
        y1 = y+row;
        
        for (int col = 0; col < MICROBIT_FONT_WIDTH; col++)
        {
            // Update our X co-ord write position
            x1 = x+col;
            
            if (x1 < width && y1 < height)
                this->bitmap[y1*width+x1] = (v & (0x08 >> col)) ? 255 : 0;
        }
    }  
}


/**
  * Shifts the pixels in this Image a given number of pixels to the Left.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftLeft(int n)
{
    uint8_t *p = bitmap;
    int pixels = width-n;
    
    if (n <= 0 || n > width)
        return;


    for (int y = 0; y < height; y++)
    {
        // Copy, and blank fill the rightmost column.
        memcpy(p, p+n, pixels);
        memclr(p+pixels, n);
        p += width;
    }        
}

/**
  * Shifts the pixels in this Image a given number of pixels to the Right.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftRight(int n)
{
    uint8_t *p = bitmap;
    int pixels = width-n;
    
    if (n <= 0 || n > width)
        return;

    for (int y = 0; y < height; y++)
    {
        // Copy, and blank fill the leftmost column.
        memcpy(p+n, p, pixels);
        memclr(p, n);
        p += width;
    }        
}


/**
  * Shifts the pixels in this Image a given number of pixels to Upward.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftUp(int n)
{
    uint8_t *pOut, *pIn;
   
    if (n <= 0 || n > height)
        return;
    
    pOut = bitmap;
    pIn = bitmap+width*n;
    
    for (int y = 0; y < height; y++)
    {
        // Copy, and blank fill the leftmost column.
        if (y < height-n)
            memcpy(pOut, pIn, width);
        else
            memclr(pOut, width);
             
        pIn += width;
        pOut += width;
    }        
}


/**
  * Shifts the pixels in this Image a given number of pixels to Downward.
  *
  * @param n The number of pixels to shift.
  */
void MicroBitImage::shiftDown(int n)
{
    uint8_t *pOut, *pIn;
   
    if (n <= 0 || n > height)
        return;
    
    pOut = bitmap + width*(height-1);
    pIn = pOut - width*n;
    
    for (int y = 0; y < height; y++)
    {
        // Copy, and blank fill the leftmost column.
        if (y < height-n)
            memcpy(pOut, pIn, width);
        else
            memclr(pOut, width);
             
        pIn -= width;
        pOut -= width;
    }        
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
