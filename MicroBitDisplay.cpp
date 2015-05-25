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
    if (animationMode == ANIMATION_MODE_NONE)
        return;
        
    if(++animationTick == animationDelay)
    {
        animationTick = 0;
        
        if (animationMode == ANIMATION_MODE_SCROLL_TEXT)
            this->updateScrollText();
        
        if (animationMode == ANIMATION_MODE_PRINT_TEXT)
            this->updatePrintText();

        if (animationMode == ANIMATION_MODE_SCROLL_IMAGE)
            this->updateScrollImage();
            
    }
}

void MicroBitDisplay::sendEvent(int eventCode)
{
    MicroBitEvent *evt = new MicroBitEvent();
    evt->source = id;
    evt->context = NULL;
    evt->timestamp = ticks;
    evt->value = eventCode;
    
    uBit.MessageBus.send(evt);
}

/**
  * Internal scrollText update method. 
  * Shift the screen image by one pixel to the left. If necessary, paste in the next char.
  */   
void MicroBitDisplay::updateScrollText()
{    
    image.shiftLeft(1);
    scrollingPosition++;
    
    if (scrollingPosition == width)
    {        
        scrollingPosition = 0;
        
        image.print(scrollingChar < scrollingText.length() ? scrollingText.charAt(scrollingChar) : ' ',width,0);

        if (scrollingChar > scrollingText.length())
        {
            animationMode = ANIMATION_MODE_NONE;
            this->sendEvent(MICROBIT_DISPLAY_EVT_SCROLLTEXT_COMPLETE);
            return;
        }
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
        this->sendEvent(MICROBIT_DISPLAY_EVT_PRINTTEXT_COMPLETE);
        return;
    }
    
    printingChar++;
}

/**
  * Internal scrollImage update method. 
  * Paste the stored bitmap at the appropriate point.
  */   
void MicroBitDisplay::updateScrollImage()
{        
    if(image.paste(scrollingImage, scrollingImagePosition, 0, 0) == 0)
    {
        animationMode = ANIMATION_MODE_NONE;  
        this->sendEvent(MICROBIT_DISPLAY_EVT_SCROLLIMAGE_COMPLETE);        
        return;
    }

    scrollingImagePosition += scrollingImageStride;
}



void MicroBitDisplay::resetAnimation(int delay)
{
    // Reset any ongoing animation.
    // Clear the display and setup the animation timers.
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
  * Uses the given delay between characters.
  * Returns immediately, and executes the animation asynchronously.
  *
  * @param s The string to display.
  * @param delay The time to delay between characters, in timer ticks.
*/
void MicroBitDisplay::printStringAsync(ManagedString s, int delay)
{
    this->resetAnimation(delay);
    
    this->printingChar = 0;
    this->printingText = s;
    
    animationMode = ANIMATION_MODE_PRINT_TEXT;
}

/**
  * Prints the given string to the display, one character at a time.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param s The string to display.
  * @param delay The time to delay between characters, in timer ticks.
  */
void MicroBitDisplay::printString(ManagedString s, int delay)
{
    // Start the effect.
    this->printStringAsync(s, delay);
    
    // Wait for completion.
    // TODO: We're polling here for now, but should really block on an event here.
    while (animationMode == ANIMATION_MODE_PRINT_TEXT)
        uBit.sleep(100);
}


/**
  * Scrolls the given string to the display, from right to left.
  * Uses the given delay between characters.
  * Returns immediately, and executes the animation asynchronously.
  *
  * @param s The string to display.
  * @param delay The time to delay between characters, in timer ticks.
  */
void MicroBitDisplay::scrollStringAsync(ManagedString s, int delay)
{
    this->resetAnimation(delay);
    
    this->scrollingPosition = width-1;
    this->scrollingChar = 0;
    this->scrollingText = s;
    
    animationMode = ANIMATION_MODE_SCROLL_TEXT;
}

/**
  * Scrolls the given string to the display, from right to left.
  * Uses the given delay between characters.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param s The string to display.
  * @param delay The time to delay between characters, in timer ticks.
  */
void MicroBitDisplay::scrollString(ManagedString s, int delay)
{
    // Start the effect.
    this->scrollStringAsync(s, delay);
    
    // Wait for completion.
    // TODO: We're polling here for now, but should really block on an event here.
    while (animationMode == ANIMATION_MODE_SCROLL_TEXT)
        uBit.sleep(100);
}


/**
  * Scrolls the given image across the display, from right to left.
  * Returns immediately, and executes the animation asynchronously.      
  *
  * @param image The image to display.
  * @param delay The time to delay between each scroll update, in timer ticks.
  * @param stride The number of pixels to move in each quantum.
  */
void MicroBitDisplay::scrollImageAsync(MicroBitImage image, int delay, int stride)
{
    this->resetAnimation(delay);

    this->scrollingImageStride = stride;
    this->scrollingImage = image;
    
    animationMode = ANIMATION_MODE_SCROLL_IMAGE;
}

/**
  * Scrolls the given image across the display, from right to left.
  * Blocks the calling thread until all the text has been displayed.
  *
  * @param image The image to display.
  * @param delay The time to delay between each scroll update, in timer ticks.
  * @param stride The number of pixels to move in each quantum.
  */
void MicroBitDisplay::scrollImage(MicroBitImage image, int delay, int stride)
{
    // Start the effect.
    this->scrollImageAsync(image, delay, stride);
    
    // Wait for completion.
    // TODO: We're polling here for now, but should really block on an event here.
    while (animationMode == ANIMATION_MODE_SCROLL_IMAGE)
        uBit.sleep(100);
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


