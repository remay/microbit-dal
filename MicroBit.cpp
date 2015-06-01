#include "inc/MicroBit.h"

char MICROBIT_BLE_DEVICE_NAME[] = "BBC MicroBit [xxxxx]";
char MICROBIT_BLE_MANUFACTURER[] = "The Cast of W1A";
char MICROBIT_BLE_MODEL[] = "Microbit SB2";
char MICROBIT_BLE_SERIAL[] = "SN1";
char MICROBIT_BLE_HARDWARE_VERSION[] = "0.2";
char MICROBIT_BLE_FIRMWARE_VERSION[] = "1.1";
char MICROBIT_BLE_SOFTWARE_VERSION[] = "1.0";

/**
  * Constructor. 
  * Create a representation of a MicroBit device.
  * @param messageBus callback function to receive MicroBitMessageBus events.
  */
MicroBit::MicroBit() : 
    flags(0x00),
    i2c(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL),
    MessageBus(), 
    display(MICROBIT_ID_DISPLAY, 5, 5),
    leftButton(MICROBIT_ID_LEFT_BUTTON,MICROBIT_PIN_LEFT_BUTTON),
    rightButton(MICROBIT_ID_RIGHT_BUTTON,MICROBIT_PIN_RIGHT_BUTTON),
    accelerometer(MICROBIT_ID_ACCELEROMETER, MMA8653_DEFAULT_ADDR),
    compass(MICROBIT_ID_COMPASS, MAG3110_DEFAULT_ADDR)
{   
}

/**
  * Post constructor initialisation method.
  * After *MUCH* pain, it's noted that the BLE stack can't be brought up in a 
  * static context, so we bring it up here rather than in the constructor.
  * n.b. This method *must* be called in main() or later, not before.
  */
void MicroBit::init()
{   
#ifdef MICROBIT_BLE
    // Start the BLE stack.        
    ble = new BLEDevice();
    
    ble->init();
 
    // Add our auxiliary services.
    ble_firmware_update_service = new MicroBitDFUService(*ble);
    ble_device_information_service = new DeviceInformationService(*ble, MICROBIT_BLE_MANUFACTURER, MICROBIT_BLE_MODEL, MICROBIT_BLE_SERIAL, MICROBIT_BLE_HARDWARE_VERSION, MICROBIT_BLE_FIRMWARE_VERSION, MICROBIT_BLE_SOFTWARE_VERSION);
    
    // Compute our auto-generated MicroBit device name.
    ble_firmware_update_service->getName(MICROBIT_BLE_DEVICE_NAME+14);
    
    // Setup advertising.
    ble->accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)MICROBIT_BLE_DEVICE_NAME, sizeof(MICROBIT_BLE_DEVICE_NAME));
    ble->setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble->setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000));
    ble->startAdvertising();   

#else

    ble = NULL;
    ble_firmware_update_service = NULL;
    ble_device_information_service = NULL;

#endif

    // Start refreshing the Matrix Display
    systemTicker.attach(this, &MicroBit::systemTick, MICROBIT_DISPLAY_REFRESH_PERIOD);     
}

/**
  * Delay for the givne amount of time.
  * If the scheduler is running, this will deschedule the current fiber and perform
  * a power efficent, concurrent sleep operation.
  * If the scheduler is disabled or we're running in an interrupt context, this
  * will revert to a busy wait. 
  */
void MicroBit::sleep(int milliseconds)
{
    if (flags & MICROBIT_FLAG_SCHEDULER_RUNNING)
        fiber_sleep(milliseconds);
    else
        wait_ms(milliseconds);
}

/**
  * Generate a randoim number in the given range.
  * We use the NRF51822 in built random number generator here
  * TODO: Determine if we want to, given its relatively high power consumption!
  *
  * @return A random, natural number between 0 and the the given maximum value.
  */
int MicroBit::random(int max)
{
    // Start the Random number generator. No need to leave it running... I hope. :-)
    NRF_RNG->TASKS_START = 1;
    
    // Clear the VALRDY EVENT
    NRF_RNG->EVENTS_VALRDY = 0;
    
    // Wait for a number ot be generated.
    while ( NRF_RNG->EVENTS_VALRDY == 0);
    
    // Disable the generator to save power.
    NRF_RNG->TASKS_STOP = 1;
    
    // Set output according to the random value
    return ((int) NRF_RNG->VALUE) % (max+1);
}


/**
  * Period callback. Used by MicroBitDisplay, FiberScheduler and I2C sensors to
  * provide a power efficient sense of time.
  *
  */
void MicroBit::systemTick()
{
    // Refresh the matric display, and update animations, if we need to.
    if (uBit.flags & MICROBIT_FLAG_DISPLAY_RUNNING)
        uBit.display.strobeUpdate();
                
    // Update Accelerometer if needed.
    if (uBit.flags & MICROBIT_FLAG_ACCELEROMETER_RUNNING)
        uBit.accelerometer.tick();
 
    // Update Accelerometer if needed.
    if (uBit.flags & MICROBIT_FLAG_COMPASS_RUNNING)
        uBit.compass.tick();
 
    // Scheduler callback. We do this here just as a single timer is more efficient. :-)
    if (uBit.flags & MICROBIT_FLAG_SCHEDULER_RUNNING)
        scheduler_tick();  
}


/**
  * Determine the time since this MicroBit was last reset.
  *
  * @return The time since the last reset, in milliseconds.
  */
unsigned long MicroBit::systemTime()
{
    return ticks;
}
