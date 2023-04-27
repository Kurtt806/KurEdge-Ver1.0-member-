#include <Arduino.h>



//*****************************************
/*
 * CHÚ Ý TẤT CẢ THÔNG TIN CÓ TRONG NÀY !!!
 * ---------------------------------------
 * BOARD     |  Servo1  |  Servo2   |
 * ---------------------------------------
 * KurEdge   |    CH1   |    CH2    |
 * ---------------------------------------
 * SERIAL SPEED = 115200
 * UPLOAD SPEED = 921600
 * FRAMEWORK    = arduino
 * 
 * _____BOARD_1_____
 * #define MANAGER_WIFI_NAME_AP "KUR_BOARD_63636"
 * #define MANAGER_WIFI_PASS_AP "63636kur"
 * _____BOARD_2_____
 * #define MANAGER_WIFI_NAME_AP "KUR_BOARD_79574"
 * #define MANAGER_WIFI_PASS_AP "79574kur"
 * _____BOARD_3_____
 * #define MANAGER_WIFI_NAME_AP "KUR_BOARD_36373"
 * #define MANAGER_WIFI_PASS_AP "36373kur"
 * _____BOARD_4_____
 * 
 *  
 * _____BOARD_5_____
 * 
 * 
 */
//*****************************************

//*---THAY ĐỔI SSID VÀ PASS MANAGER--------

#define MANAGER_WIFI_NAME_AP "KUR_BOARD_36373"
#define MANAGER_WIFI_PASS_AP "36373kur"

//*---THAY ĐỔI THÔNG SỐ SERVO--------------

#define START_DEGREE_CH1               90
#define SERVO_MIN_LIMIT_DEGREE_CH1     0
#define SERVO_MAX_LIMIT_DEGREE_CH1     180
#define SERVO_SPEED_DEGREE_CH1         9999

#define START_DEGREE_CH2               90
#define SERVO_MIN_LIMIT_DEGREE_CH2     0
#define SERVO_MAX_LIMIT_DEGREE_CH2     180
#define SERVO_SPEED_DEGREE_CH2         9999
//*----------------------------------------
#define APP_DEBUG
#include <KurEdge.h>
void setup()
{
  KurEdge.begin();
}
void loop()
{
  KurEdge.run();
}
 

/*
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

void setup()
{
  Serial.begin(115200);
}
void loop()
{

}
*/





