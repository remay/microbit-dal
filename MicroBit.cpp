#include "inc/MicroBit.h"
#include "URIBeaconConfigService.h"

extern void dfuCallbackFn();

char MICROBIT_BLE_DEVICE_NAME[] = "BBC MicroBit Prototype";
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
MicroBit::MicroBit() //:display(MICROBIT_ID_DISPLAY, 5, 5)
  //leftButton(MICROBIT_ID_LEFT_BUTTON,MICROBIT_PIN_LEFT_BUTTON)
  //i2c(MICROBIT_PIN_SDA, MICROBIT_PIN_SCL)
{
    display = new MicroBitDisplay(MICROBIT_ID_DISPLAY, 5, 5); 

    // Set up BLE stack.    

    ble = new BLEDevice();
    ble->init();
    
    /* Setup auxiliary services. */
    ble_firmware_update_service = new DFUService(*ble);
    ble_device_information_service = new DeviceInformationService(*ble, MICROBIT_BLE_MANUFACTURER, MICROBIT_BLE_MODEL, MICROBIT_BLE_SERIAL, MICROBIT_BLE_HARDWARE_VERSION, MICROBIT_BLE_FIRMWARE_VERSION, MICROBIT_BLE_SOFTWARE_VERSION);
     
    /* Setup advertising. */
    ble->accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)MICROBIT_BLE_DEVICE_NAME, sizeof(MICROBIT_BLE_DEVICE_NAME));
    //ble.setDeviceName(reinterpret_cast<uint8_t *>(&MICROBIT_BLE_DEVICE_NAME));
    ble->setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

    ble->setAdvertisingInterval(Gap::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000));
    
    ble->startAdvertising();  
}
