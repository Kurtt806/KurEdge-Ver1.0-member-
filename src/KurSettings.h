#if defined(ESP8266)
#define SERVO1_PIN                      12 
#define SERVO2_PIN                      13 
#define SPEED_IN_PIN                    A0

#define BOARD_BUTTON_PIN                0
#define BOARD_BUTTON_ACTIVE             HIGH

#define BOARD_LED_PIN                   14
#define BOARD_LED_INVERSE               false
#define BOARD_LED_BRIGHTNESS            255

#elif defined(ESP32)
#define SERVO1_PIN                      12 
#define SERVO2_PIN                      13 
#define SPEED_IN_PIN                    A0

#define BOARD_BUTTON_PIN                0
#define BOARD_BUTTON_ACTIVE             HIGH

#define BOARD_LED_PIN                   14
#define BOARD_LED_INVERSE               false
#define BOARD_LED_BRIGHTNESS            255
#endif
//-------------------------------------------

#define BUTTON_HOLD_TIME_INDICATION     3000
#define BUTTON_HOLD_TIME_ACTION         6000
#define BUTTON_PRESS_TIME_ACTION        50

#define BOARD_PWM_MAX                   1023
#define REFRESH_DURATION                1000
