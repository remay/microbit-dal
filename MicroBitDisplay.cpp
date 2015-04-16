/**
  * Class implementation for the MicroBitDisplay.
  *
  * An MicroBitDisplay represents the LED matrix array on the MicroBit device.
  */
#include "inc/MicroBitDisplay.h"

    /**
      * Provides the mapping from Matrix ROW/COL to a linear X/Y buffer. 
      * It's arranged such that matrixMap[col, row] provides the [x,y] screen co-ord.     
      */
#ifdef MICROBUG_REFERENCE_DEVICE
    const MatrixPoint MicroBitDisplay::matrixMap[MICROBIT_DISPLAY_COLUMN_COUNT][MICROBIT_DISPLAY_ROW_COUNT] = 
    {   {MatrixPoint(0,0),MatrixPoint(0,1),MatrixPoint(0,2), MatrixPoint(0,3), MatrixPoint(0,4)},
        {MatrixPoint(1,0),MatrixPoint(1,1),MatrixPoint(1,2), MatrixPoint(1,3), MatrixPoint(1,4)},
        {MatrixPoint(2,0),MatrixPoint(2,1),MatrixPoint(2,2), MatrixPoint(2,3), MatrixPoint(2,4)},
        {MatrixPoint(3,0),MatrixPoint(3,1),MatrixPoint(3,2), MatrixPoint(3,3), MatrixPoint(3,4)},
        {MatrixPoint(4,0),MatrixPoint(4,1),MatrixPoint(4,2), MatrixPoint(4,3), MatrixPoint(4,4)} 
    };
#endif

#ifdef MICROBIT_3X9
    const MatrixPoint MicroBitDisplay::matrixMap[MICROBIT_DISPLAY_COLUMN_COUNT][MICROBIT_DISPLAY_ROW_COUNT] = 
    {   
        {MatrixPoint(0,4),MatrixPoint(0,3),MatrixPoint(1,1)},
        {MatrixPoint(1,4),MatrixPoint(4,2),MatrixPoint(0,1)},
        {MatrixPoint(2,4),MatrixPoint(3,2),MatrixPoint(4,0)},  
        {MatrixPoint(3,4),MatrixPoint(2,2),MatrixPoint(3,0)},
        {MatrixPoint(4,4),MatrixPoint(1,2),MatrixPoint(2,0)},
        {MatrixPoint(4,3),MatrixPoint(0,2),MatrixPoint(1,0)},
        {MatrixPoint(3,3),MatrixPoint(4,1),MatrixPoint(0,0)},
        {MatrixPoint(2,3),MatrixPoint(3,1),MatrixPoint(NO_CONN,NO_CONN)},
        {MatrixPoint(1,3),MatrixPoint(2,1),MatrixPoint(NO_CONN,NO_CONN)}
    };
#endif

#ifdef MICROBIT_9X3
    const MatrixPoint MicroBitDisplay::matrixMap[MICROBIT_DISPLAY_COLUMN_COUNT][MICROBIT_DISPLAY_ROW_COUNT] = 
    {   
        {MatrixPoint(0,4), MatrixPoint(1,4), MatrixPoint(2,4), MatrixPoint(3,4), MatrixPoint(4,4), MatrixPoint(4,3), MatrixPoint(3,3), MatrixPoint(2,3), MatrixPoint(1,3)},
        {MatrixPoint(0,3), MatrixPoint(4,2), MatrixPoint(3,2), MatrixPoint(2,2), MatrixPoint(1,2), MatrixPoint(0,2), MatrixPoint(4,1), MatrixPoint(3,1), MatrixPoint(2,1)},
        {MatrixPoint(1,1), MatrixPoint(0,1), MatrixPoint(4,0), MatrixPoint(3,0), MatrixPoint(2,0), MatrixPoint(1,0), MatrixPoint(0,0), MatrixPoint(NO_CONN,NO_CONN), MatrixPoint(NO_CONN,NO_CONN)}        
    };
#endif

 
/**
  * Constructor.
  * Create a Point representation of an LED on a matrix
  * Used to handle non-linear matrix layouts.
  */
MatrixPoint::MatrixPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

   
/**
  * Constructor. 
  * Create a representation of a display of a given size.
  * The display is initially blank.
  * 
  * @param x the width of the display in pixels.
  * @param y the height of the display in pixels.     
  */
MicroBitDisplay::MicroBitDisplay(int id, int x, int y) : columnPins(MICROBIT_DISPLAY_COLUMN_PINS), rowPins(MICROBIT_DISPLAY_ROW_PINS), image(x*2,y)
{
    this->id = id;
    this->width = x;
    this->height = y;
    this->brightness = MICROBIT_DEFAULT_BRIGHTNESS;
    this->strobeRow = 0;
    
    this->scrollingDelay = 0;
    this->scrollingTick = 0;
    this->scrollingPosition = 0;
    this->scrollingChar = 0;
    this->scrollingText = NULL;

    this->strobe.attach(this, &MicroBitDisplay::strobeUpdate, MICROBIT_DISPLAY_REFRESH_PERIOD);
}

/**
  * Internal frame update method, used to strobe the display.
  * TODO: Write a more efficient, complementary variation of this method for the case where 
  * MICROBIT_DISPLAY_ROW_COUNT > MICROBIT_DISPLAY_COLUMN_COUNT.
  */   
void MicroBitDisplay::strobeUpdate()
{
    // TODO: Cache row data for future use, so we don't recompute so often?
    int rowdata;
    int coldata;

    // move on to next row.
    strobeRow = (strobeRow+1) % MICROBIT_DISPLAY_ROW_COUNT;
    rowdata = 1 << strobeRow;

    // Here we go. Calculate the data for this row.
    coldata = 0;
    for (int i = 0; i<MICROBIT_DISPLAY_COLUMN_COUNT; i++)
    {
        if(image.getPixelValue(matrixMap[i][strobeRow].x, matrixMap[i][strobeRow].y))
            coldata |= (1 << i);
    }

    // Write to the matrix display.
    columnPins.write(0xff);    
    rowPins.write(rowdata);
    columnPins.write(~coldata);
    
    // Update Scrolling Text if we need to.
    if (scrollingText != NULL && (++scrollingTick == scrollingDelay))
    {
        scrollingTick = 0;
        this->updateScroll();
    }
}

/**
  * Internal scrollText update method. 
  * Shift the screen image by one pixel to the left. If necessary, paste in the next char.
  */   
void MicroBitDisplay::updateScroll()
{    
    image.shiftLeft(1);
    scrollingPosition++;
    
    if (scrollingPosition == width)
    {        
        scrollingPosition=0;
        
        image.print(scrollingChar < scrollTextLength ? scrollingText[scrollingChar] : ' ',width,0);

        if (scrollingChar > scrollTextLength)
            scrollingText = NULL;
        
        scrollingChar++;
   }
    
}

/**
  * Prints the given character to the display.
  *
  * @param c The character to display.
  */
void MicroBitDisplay::print(char c)
{
    image.print(c, 0, 0);
}

/**
  * Prints the given string to the display, one character at a time.
  * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
  *
  * @param str The string to display.
  */
void MicroBitDisplay::printString(char *str)
{
    // TODO: 
}

/**
  * Prints the given string to the display, one character at a time.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param str The string to display.
  * @param delay The time to delay between characters, in milliseconds.
  */
void MicroBitDisplay::printString(char *str, int delay)
{
    // TODO:
}

/**
  * Scrolls the given string to the display, from right to left.
  * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param str The string to display.
  */
void MicroBitDisplay::scrollString(char *str)
{
    this->scrollString(str, MICROBIT_DEFAULT_SCROLL_SPEED);
}

/**
  * Scrolls the given string to the display, from right to left.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param str The string to display.
  * @param delay The time to delay between characters, in milliseconds.
  */
void MicroBitDisplay::scrollString(char *str, int delay)
{
    this->image.clear();
    this->scrollingDelay = delay;
    this->scrollingTick = delay-1;
    this->scrollingPosition = width-1;
    this->scrollingChar = 0;
    this->scrollingText = str;
    this->scrollTextLength = strlen(str);
}

/**
  * Scrolls the given image across the display, from right to left.
  * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param image The image to display.
  */
void MicroBitDisplay::scrollImage(MicroBitImage *image)
{
    // TODO:
}

/**
  * Scrolls the given image across the display, from right to left.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param image The image to display.
  * @param delay The time to delay between characters, in milliseconds.
  */
void MicroBitDisplay::scrollImage(MicroBitImage *image, int delay)
{
    // TODO:
}

 /**
  * Sets the display brightness to the specified level.
  * @param b The brightness to set the brightness to, in the range 0..255.
  */    
void MicroBitDisplay::setBrightness(int b)
{
    this->brightness = b;
}

 /**
  * Tests the brightness of this display.
  * @return the brightness of this display, in the range 0..255.
  */    
int MicroBitDisplay::getBrightness()
{
    return this->brightness;
}

