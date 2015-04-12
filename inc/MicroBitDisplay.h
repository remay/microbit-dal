/**
  * Class definition for a MicroBitDisplay.
  *
  * An MicroBitDisplay represents the LED matrix array on the MicroBit device.
  */
  
#ifndef MICROBIT_DISPLAY_H
#define MICROBIT_DISPLAY_H

#define MICROBIT_DEFAULT_SCROLL_SPEED       1000
#define MICROBIT_DEFAULT_BRIGHTNESS         128

#define MICROBIT_DISPLAY_COLUMN_COUNT       5
#define MICROBIT_DISPLAY_COLUMN_PINS        P0_24, P0_25, P0_28, P0_29, P0_30
#define MICROBIT_DISPLAY_ROW_COUNT          5
#define MICROBIT_DISPLAY_ROW_PINS           P0_0, P0_1, P0_2, P0_3, P0_4


#include "mbed.h"
#include "MicroBitImage.h"

struct MatrixPoint
{
    int x;
    int y;
    
    MatrixPoint(int x, int y);
};


class MicroBitDisplay
{
    int id;
    int brightness;
    int strobeRow;
    Ticker strobe;
    BusOut columnPins;
    BusOut rowPins;

    static const MatrixPoint matrixMap[MICROBIT_DISPLAY_COLUMN_COUNT][MICROBIT_DISPLAY_ROW_COUNT];
    
    public:
    MicroBitImage image;
 


    /**
      * Constructor. 
      * Create a representation of a display of a given size.
      * The display is initially blank.
      * 
      * @param x the width of the display in pixels.
      * @param y the height of the display in pixels.     
      */
    MicroBitDisplay(int id, int x, int y);

    /**
      * Frame update method, invoked periodically to strobe the display.
      */   
    void strobeUpdate();

    /**
      * Prints the given character to the display.
      *
      * @param c The character to display.
      */
    void print(char c);

    /**
      * Prints the given string to the display, one character at a time.
      * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
      *
      * @param str The string to display.
      */
    void printString(char *str);

    /**
      * Prints the given string to the display, one character at a time.
      * Uses the given delay between characters.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param str The string to display.
      * @param delay The time to delay between characters, in milliseconds.
      */
    void printString(char *str, int delay);

    /**
      * Scrolls the given string to the display, from right to left.
      * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param str The string to display.
      */
    void scrollString(char *str);

    /**
      * Scrolls the given string to the display, from right to left.
      * Uses the given delay between characters.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param str The string to display.
      * @param delay The time to delay between characters, in milliseconds.
      */
    void scrollString(char *str, int delay);

    /**
      * Scrolls the given image across the display, from right to left.
      * Uses the default delay between characters as defined by DEFAULT_SCROLLTEXT_SPEED.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param image The image to display.
      */
    void scrollImage(MicroBitImage *image);

    /**
      * Scrolls the given image across the display, from right to left.
      * Uses the given delay between characters.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param image The image to display.
      * @param delay The time to delay between characters, in milliseconds.
      */
    void scrollImage(MicroBitImage *image, int delay);

     /**
      * Sets the display brightness to the specified level.
      * @param b The brightness to set the brightness to, in the range 0..255.
      */    
    void setBrightness(int b);
    
     /**
      * Tests the brightness of this display.
      * @return the brightness of this display, in the range 0..255.
      */    
    int getBrightness();    
};

#endif

