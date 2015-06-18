#ifndef MICROBIT_DISPLAY_H
#define MICROBIT_DISPLAY_H

/**
  * User definable constants
  */
#define MICROBIT_DISPLAY_ROTATION_0         0
#define MICROBIT_DISPLAY_ROTATION_90        1
#define MICROBIT_DISPLAY_ROTATION_180       2
#define MICROBIT_DISPLAY_ROTATION_270       3


/**
  * Core Confuration settings.
  */

#define MICROBIT_SB2
#define MICROBIT_DISPLAY_REFRESH_PERIOD     0.006


/**
  * Default parameters.
  */
#define MICROBIT_DEFAULT_SCROLL_SPEED       90
#define MICROBIT_DEFAULT_PRINT_SPEED        1200
#define MICROBIT_DEFAULT_SCROLL_STRIDE      -1
#define MICROBIT_DISPLAY_MAX_BRIGHTNESS     255
#define MICROBIT_DEFAULT_BRIGHTNESS         (MICROBIT_DISPLAY_MAX_BRIGHTNESS/2)

/**
  * MessageBus Event Codes
  */
#define MICROBIT_DISPLAY_EVT_SCROLLTEXT_COMPLETE         1
#define MICROBIT_DISPLAY_EVT_PRINTTEXT_COMPLETE          2
#define MICROBIT_DISPLAY_EVT_SCROLLIMAGE_COMPLETE        3


/**
  * I/O configurations for common devices.
  */
#ifdef MICROBUG_REFERENCE_DEVICE
#define MICROBIT_DISPLAY_ROW_COUNT          5
#define MICROBIT_DISPLAY_ROW_PINS           P0_0, P0_1, P0_2, P0_3, P0_4
#define MICROBIT_DISPLAY_COLUMN_COUNT       5
#define MICROBIT_DISPLAY_COLUMN_PINS        P0_24, P0_25, P0_28, P0_29, P0_30
#endif

#ifdef MICROBIT_3X9
#define MICROBIT_DISPLAY_ROW_COUNT          3
#define MICROBIT_DISPLAY_ROW_PINS           P0_12, P0_13, P0_14
#define MICROBIT_DISPLAY_COLUMN_COUNT       9
#define MICROBIT_DISPLAY_COLUMN_PINS        P0_15, P0_16, P0_17, P0_18, P0_19, P0_24, P0_25, P0_28, P0_29
#endif

#ifdef MICROBIT_SB1
#define MICROBIT_DISPLAY_ROW_COUNT          9
#define MICROBIT_DISPLAY_ROW_PINS           P0_15, P0_16, P0_17, P0_18, P0_19, P0_24, P0_25, P0_28, P0_29
#define MICROBIT_DISPLAY_COLUMN_COUNT       3
#define MICROBIT_DISPLAY_COLUMN_PINS        P0_12, P0_13, P0_14
#endif

#ifdef MICROBIT_SB2
#define MICROBIT_DISPLAY_ROW_COUNT          3
#define MICROBIT_DISPLAY_ROW_PINS           P0_13, P0_14, P0_15
#define MICROBIT_DISPLAY_COLUMN_COUNT       9
#define MICROBIT_DISPLAY_COLUMN_PINS        P0_4, P0_5, P0_6, P0_7, P0_8, P0_9, P0_10, P0_11, P0_12
#endif

#define SPACING 1

#include "mbed.h"
#include "MicroBit.h"

enum AnimationMode {
    ANIMATION_MODE_NONE,
    ANIMATION_MODE_SCROLL_TEXT,
    ANIMATION_MODE_PRINT_TEXT,
    ANIMATION_MODE_SCROLL_IMAGE
};

struct MatrixPoint {
    int x;
    int y;

    MatrixPoint(int x, int y);
};

/**
  * Class definition for a MicroBitDisplay.
  *
  * A MicroBitDisplay represents the LED matrix array on the MicroBit device.
  */
class MicroBitDisplay
{
    int id;
    int width;
    int height;
    int brightness;
    int strobeRow;
    int rotation;
    BusOut columnPins;
    DynamicPwm* rowDrive;
    int strobeCount;
    MicroBitFont font;
    
    //
    // State used by all animation routines.
    //

    // The animation mode that's currently running (if any)
    AnimationMode animationMode;

    // The time (in ticks) between each frame update.
    int animationDelay;

    // The time (in ticks) since the frame update.
    int animationTick;

    // Stop playback of any animations
    void stopAnimation(int delay);

    //
    // State for scrollString() method.
    // This is a surprisingly intricate method.
    //
    // The text being displayed.
    ManagedString scrollingText;

    // The index of the character currently being displayed.
    int scrollingChar;

    // The number of pixels the current character has been shifted on the display.
    int scrollingPosition;

    //
    // State for printString() method.
    //
    // The text being displayed. NULL if no message is scheduled for playback.
    // We *could* get some reuse in here with the scroll* variables above,
    // but best to keep it clean in case kids try concurrent operation (they will!),
    // given the small RAM overhead needed to maintain orthogonality.
    ManagedString printingText;

    // The index of the character currently being displayed.
    int printingChar;

    //
    // State for scrollImage() method.
    //
    // The image being displayed.
    MicroBitImage scrollingImage;

    // The number of pixels the image has been shifted on the display.
    int scrollingImagePosition;

    // The number of pixels the image is shifted on the display in each quantum.
    int scrollingImageStride;

    // Flag to indicate if image has been rendered to screen yet (or not)
    bool scrollingImageRendered;

    static const MatrixPoint matrixMap[MICROBIT_DISPLAY_COLUMN_COUNT][MICROBIT_DISPLAY_ROW_COUNT];

    // Internal methods to handle animation.
    
    /**
      * Resets the current given animation.
      * @param delay the delay after which the animation is reset.
      */
    void resetAnimation(int delay);
    
    /**
      *  Periodic callback, that we use to perform any animations we have running.
      */
    void animationUpdate();
    
    /**
      * Internal scrollText update method. 
      * Shift the screen image by one pixel to the left. If necessary, paste in the next char.
      */  
    void updateScrollText();
    
    /**
      * Internal printText update method. 
      * Paste in the next char in the string.
      */  
    void updatePrintText();
    
    /**
      * Internal scrollImage update method. 
      * Paste the stored bitmap at the appropriate point.
      */  
    void updateScrollImage();
    
    /**
      * Broadcasts an event onto the shared MessageBus
      * @param eventCode The ID of the event that has occurred.
      */
    void sendEvent(int eventcode);


public:
    // The mutable bitmap buffer being rendered to the LED matrix.
    MicroBitImage image;

    /**
      * Constructor.
      * Create a representation of a display of a given size.
      * The display is initially blank.
      *
      * @param x the width of the display in pixels.
      * @param y the height of the display in pixels.
      * 
      * Example:
      * @code 
      * MicroBitDisplay display(MICROBIT_ID_DISPLAY, 5, 5),
      * @endcode
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
      * 
      * Example:
      * @code 
      * uBit.display.print('p');
      * @endcode
      */
    void print(char c);

    /**
      * Prints the given string to the display, one character at a time.
      * Uses the given delay between characters.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param s The string to display.
      * @param delay The time to delay between characters, in timer ticks.
      * 
      * Example:
      * @code 
      * uBit.display.printString("abc123",400);
      * @endcode
      */
    void printString(ManagedString s, int delay = MICROBIT_DEFAULT_PRINT_SPEED);

    /**
      * Prints the given string to the display, one character at a time.
      * Uses the given delay between characters.
      * Returns immediately, and executes the animation asynchronously.
      *
      * @param s The string to display.
      * @param delay The time to delay between characters, in timer ticks.
      * 
      * Example:
      * @code 
      * uBit.display.printStringAsync("abc123",400);
      * @endcode
      */
    void printStringAsync(ManagedString s, int delay = MICROBIT_DEFAULT_PRINT_SPEED);


    /**
      * Scrolls the given string to the display, from right to left.
      * Uses the given delay between characters.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param s The string to display.
      * @param delay The time to delay between characters, in timer ticks.
      * 
      * Example:
      * @code 
      * uBit.display.scrollString("abc123",100);
      * @endcode
      */
    void scrollString(ManagedString s, int delay = MICROBIT_DEFAULT_SCROLL_SPEED);

    /**
      * Scrolls the given string to the display, from right to left.
      * Uses the given delay between characters.
      * Returns immediately, and executes the animation asynchronously.
      *
      * @param s The string to display.
      * @param delay The time to delay between characters, in timer ticks.
      * 
      * Example:
      * @code 
      * uBit.display.scrollStringAsync("abc123",100);
      * @endcode
      */
    void scrollStringAsync(ManagedString s, int delay = MICROBIT_DEFAULT_SCROLL_SPEED);


    /**
      * Scrolls the given image across the display, from right to left.
      * Blocks the calling thread until all the text has been displayed.
      *
      * @param image The image to display.
      * @param delay The time to delay between each scroll update, in timer ticks. Has a default.
      * @param stride The number of pixels to move in each quantum. Has a default. 
      * 
      * Example:
      * @code 
      * MicrobitImage i("1,1,1,1,1\n1,1,1,1,1\n"); 
      * uBit.display.scrollImage(i,100,1);
      * @endcode
      */
    void scrollImage(MicroBitImage image, int delay = MICROBIT_DEFAULT_SCROLL_SPEED, int stride = MICROBIT_DEFAULT_SCROLL_STRIDE);


    /**
      * Scrolls the given image across the display, from right to left.
      * Returns immediately, and executes the animation asynchronously.
      * @param image The image to display.
      * @param delay The time to delay between each scroll update, in timer ticks. Has a default.
      * @param stride The number of pixels to move in each quantum. Has a default.
      * 
      * Example:
      * @code 
      * MicrobitImage i("1,1,1,1,1\n1,1,1,1,1\n"); 
      * uBit.display.scrollImageAsync(i,100,1);
      * @endcode
      */
    void scrollImageAsync(MicroBitImage image, int delay = MICROBIT_DEFAULT_SCROLL_SPEED, int stride = MICROBIT_DEFAULT_SCROLL_STRIDE);

    /**
      * Sets the display brightness to the specified level.
      * @param b The brightness to set the brightness to, in the range 0..255.
      * 
      * Example:
      * @code 
      * uBit.display.setBrightness(255); //max brightness
      * @endcode
      */
    void setBrightness(int b);

    /**
      * Fetches the current brightness of this display.
      * @return the brightness of this display, in the range 0..255.
      * 
      * Example:
      * @code 
      * uBit.display.getBrightness(); //the current brightness
      * @endcode
      */
    int getBrightness();

    /**
      * Rotates the display to the given position.
      * Axis aligned values only.
      *
      * Example:
      * @code 
      * uBit.display.rotateTo(MICROBIT_DISPLAY_ROTATION_180); //rotates 180 degrees from original orientation
      * @endcode
      */
    void rotateTo(int position);
    
    /**
      * Enables the display, should only be called if the display is disabled.
      *
      * Example:
      * @code 
      * uBit.display.enable(); //reenables the display mechanics
      * @endcode
      */
    void enable();
    
    /**
      * Disables the display, should only be called if the display is enabled.
      * Display must be disabled to avoid MUXing of edge connector pins.
      *
      * Example:
      * @code 
      * uBit.display.disable(); //disables the display
      * @endcode
      */
    void disable();
    
    /**
      * Clears the current image on the display.
      * Simplifies the process, you can also use uBit.display.image.clear
      *
      * Example:
      * @code 
      * uBit.display.clear(); //clears the display
      * @endcode
      */ 
    void clear();
    
     /**
      * Displays "=(" and an accompanying status code infinitely.
      * @param statusCode the appropriate status code - 0 means no code will be displayed.
      *
      * Example:
      * @code 
      * uBit.display.error(20);
      * @endcode
      */
    void error(int statusCode);
    
    /**
      * Updates the font property of this object with the new font.
      * @param font the new font that will be used to render characters..
      */
    void setFont(MicroBitFont font);

    /**
      * Retreives the font object used for rendering characters on the display.
      */
    MicroBitFont getFont();  
};

#endif

