#ifndef BLE_EVENTS_H
#define BLE_EVENTS_H

//
// BLE BLE Event Components and their associated events
//

//
// Events that BLE devices respond to:
//
#define BLE_REMOTE_CONTROL_ID               1001
#define BLE_REMOTE_CONTROL_EVT_PLAY         0
#define BLE_REMOTE_CONTROL_EVT_PAUSE        1
#define BLE_REMOTE_CONTROL_EVT_STOP         2
#define BLE_REMOTE_CONTROL_EVT_NEXTTRACK    3
#define BLE_REMOTE_CONTROL_EVT_PREVTRACK    4
#define BLE_REMOTE_CONTROL_EVT_FORWARD      5
#define BLE_REMOTE_CONTROL_EVT_REWIND       6
#define BLE_REMOTE_CONTROL_EVT_VOLUMEUP     7
#define BLE_REMOTE_CONTROL_EVT_VOLUMEDOWN   8


#define BLE_CAMERA_ID                       1002
#define BLE_CAMERA_EVT_LAUNCH_PHOTO_MODE    0
#define BLE_CAMERA_EVT_LAUNCH_VIDEO_MODE    1
#define BLE_CAMERA_EVT_TAKE_PHOTO           2
#define BLE_CAMERA_EVT_START_VIDEO_CAPTURE  3
#define BLE_CAMERA_EVT_STOP_VIDEO_CAPTURE   4
#define BLE_CAMERA_EVT_STOP_PHOTO_MODE      5
#define BLE_CAMERA_EVT_STOP_VIDEO_MODE      6
#define BLE_CAMERA_EVT_TOGGLE_FRONT_REAR    7


#define BLE_AUDIO_RECORDER_ID                   1003
#define BLE_AUDIO_RECORDER_EVT_LAUNCH           0
#define BLE_AUDIO_RECORDER_EVT_START_CAPTURE    1
#define BLE_AUDIO_RECORDER_EVT_STOP_CAPTURE     2
#define BLE_AUDIO_RECORDER_EVT_STOP             3


#define BLE_ALERTS_ID                       1004
#define BLE_ALERT_EVT_DISPLAY_TOAST         0
#define BLE_ALERT_EVT_VIBRATE               1
#define BLE_ALERT_EVT_PLAY_SOUND            2
#define BLE_ALERT_EVT_PLAY_RINGTONE         3
#define BLE_ALERT_EVT_FIND_MY_PHONE         4
#define BLE_ALERT_EVT_ALARM1                5 
#define BLE_ALERT_EVT_ALARM2                6
#define BLE_ALERT_EVT_ALARM3                7
#define BLE_ALERT_EVT_ALARM4                8
#define BLE_ALERT_EVT_ALARM5                9
#define BLE_ALERT_EVT_ALARM6                10

//
// Events that BLE devices generate:
//
#define BLE_SIGNAL_STRENGTH_ID              1101
#define BLE_SIGNAL_STRENGTH_EVT_NO_BAR      0
#define BLE_SIGNAL_STRENGTH_EVT_ONE_BAR     1
#define BLE_SIGNAL_STRENGTH_EVT_TWO_BAR     2
#define BLE_SIGNAL_STRENGTH_EVT_THREE_BAR   3
#define BLE_SIGNAL_STRENGTH_EVT_FOUR_BAR    4

#define BLE_PLAY_CONTROLLER_ID      1102
#define BLE_BUTTON_UP               0
#define BLE_BUTTON_DOWN             1
#define BLE_BUTTON_RIGHT            2
#define BLE_BUTTON_LEFT             3
#define BLE_BUTTON_A                4
#define BLE_BUTTON_B                5
#define BLE_BUTTON_C                6
#define BLE_BUTTON_D                7

#define BLE_DEVICE_INFO_ID                  1103
#define BLE_DEVICE_ORIENTATION_LANDSCAPE    0
#define BLE_DEVICE_ORIENTATION_PORTRAIT     1
#define BLE_DEVICE_GESTURE_NONE             2
#define BLE_DEVICE_GESTURE_DEVICE_SHAKEN    3
#define BLE_DEVICE_DISPLAY_OFF              4
#define BLE_DEVICE_DISPLAY_ON               5

#endif