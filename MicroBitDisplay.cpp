/**
  * Class implementation for the MicroBitDisplay.
  *
  * An MicroBitDisplay represents the LED matrix array on the MicroBit device.
  */
#include "MicroBit.h"
#include "MicroBitMatrixMaps.h"

 
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
  * Empty String definitions
  */
static ManagedString EMPTY_STRING("");   

/**
  * Constructor. 
  * Create a representation of a display of a given size.
  * The display is initially blank.
  * 
  * @param x the width of the display in pixels.
  * @param y the height of the display in pixels.     
  */
MicroBitDisplay::MicroBitDisplay(int id, int x, int y) : strobe(), columnPins(MICROBIT_DISPLAY_COLUMN_PINS), rowPins(MICROBIT_DISPLAY_ROW_PINS), image(x*2,y)
{
    this->id = id;
    this->width = x;
    this->height = y;
    this->brightness = MICROBIT_DEFAULT_BRIGHTNESS;
    this->strobeRow = 0;
    
    animationMode = ANIMATION_MODE_NONE;
}

void MicroBitDisplay::startDisplay()
{
    this->strobe.attach(this, &MicroBitDisplay::strobeUpdate, MICROBIT_DISPLAY_REFRESH_PERIOD);
}


/**
  * Internal frame update method, used to strobe the display.
  *
  * TODO: Write a more efficient, complementary variation of this method for the case where 
  * MICROBIT_DISPLAY_ROW_COUNT > MICROBIT_DISPLAY_COLUMN_COUNT.
  */   
void MicroBitDisplay::strobeUpdate()
{   
    // TODO: Cache row data for future use, so we don't recompute so often?
    int rowdata;
    int coldata;
    
    pc.printf(":");
    
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
    columnPins.write(0xffff);    
    rowPins.write(rowdata);
    columnPins.write(~coldata);

    // Update text and image animations if we need to.
    this->animationUpdate();
        
    // Scheduler callback. We do this here just as a single timer is more efficient. :-)
    if (uBit.flags & MICROBIT_FLAG_SCHEDULER_RUNNING)
        scheduler_tick();  
}

/**
  * Periodic callback, that we use to performs any animations we have running.
  */
void
MicroBitDisplay::animationUpdate()
{   
    pc.printf(".");
    
    if (animationMode == ANIMATION_MODE_NONE)
        return;
        
    if(++animationTick == animationDelay)
    {
        pc.printf("=== MicroBitDisplay::animationUpdate ===\n");
        animationTick = 0;
        
        if (animationMode == ANIMATION_MODE_SCROLL_TEXT)
            this->updateScrollText();
        
        if (animationMode == ANIMATION_MODE_PRINT_TEXT)
            this->updatePrintText();

        if (animationMode == ANIMATION_MODE_SCROLL_IMAGE)
            this->updateScrollImage();
            
    }
}

/**
  * Internal scrollText update method. 
  * Shift the screen image by one pixel to the left. If necessary, paste in the next char.
  */   
void MicroBitDisplay::updateScrollText()
{    
    pc.printf("=== MicroBitDisplay::updateScrollText ===\n");

    image.shiftLeft(1);
    scrollingPosition++;
    
    if (scrollingPosition == width)
    {        
        scrollingPosition = 0;
        
        image.print(scrollingChar < scrollingText.length() ? scrollingText.charAt(scrollingChar) : ' ',width,0);

        if (scrollingChar > scrollingText.length())
            animationMode = ANIMATION_MODE_NONE;
        
        scrollingChar++;
   }
}

/**
  * Internal printText update method. 
  * Paste in the next char in the string.
  */   
void MicroBitDisplay::updatePrintText()
{        
    image.print(printingChar < printingText.length() ? printingText.charAt(printingChar) : ' ',0,0);

    if (printingChar > printingText.length())
    {
        animationMode = ANIMATION_MODE_NONE;   
        printingChar++;
    }
}

/**
  * Internal printText update method. 
  * Paste in the next char in the string.
  */   
void MicroBitDisplay::updateScrollImage()
{        
    // TODO: Write this!
}



void MicroBitDisplay::resetAnimation(int delay)
{
    // Reset any ongoing animation.
    // Clear the display and setup the animation timers.
    pc.printf("=== MicroBitDisplay::resetAnimation ===\n");
    animationMode = ANIMATION_MODE_NONE;

    this->image.clear();

    this->animationDelay = delay;
    this->animationTick = delay-1;
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
void MicroBitDisplay::printString(ManagedString s)
{
    this->printString(s, MICROBIT_DEFAULT_PRINT_SPEED); 
}

/**
  * Prints the given string to the display, one character at a time.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param str The string to display.
  * @param delay The time to delay between characters, in milliseconds.
  */
void MicroBitDisplay::printString(ManagedString s, int delay)
{
    this->resetAnimation(delay);
    
    this->printingChar = 0;
    this->printingText = s;
}

/**
  * Scrolls the given string to the display, from right to left.
  * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param str The string to display.
  */
void MicroBitDisplay::scrollString(ManagedString s)
{
    this->scrollString(s, MICROBIT_DEFAULT_SCROLL_SPEED);
}

/**
  * Scrolls the given string to the display, from right to left.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param str The string to display.
  * @param delay The time to delay between characters, in milliseconds.
  */
void MicroBitDisplay::scrollString(ManagedString s, int delay)
{
    pc.printf("=== MicroBitDisplay::scrollString ===\n");
    this->resetAnimation(delay);
    
    this->scrollingPosition = width-1;
    this->scrollingChar = 0;
    this->scrollingText = s;
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


