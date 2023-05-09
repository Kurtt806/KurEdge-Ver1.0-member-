
#if defined(MANAGER_WIFI_NAME_AP) && !defined(MANAGER_WIFI_PASS_AP)
MANAGER_WIFI_PASS_AP = NULL;
#endif

#if !defined(MANAGER_WIFI_NAME_AP)
#error "Please specify your MANAGER_WIFI_NAME and MANAGER_WIFI_PASS"
#endif

#if defined(OTOCORES)
#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include "ServoEasing.hpp"
ServoEasing Servo1;
ServoEasing Servo2;

int goc = 0, input = 0;
#endif

#include <Ticker.h>
Ticker ticker;
WiFiServer serverRobo(80);
WiFiClient client;

enum State
{
    MODE_SWITCH_STA,
    MODE_RUNNING,
    //MODE_SWITCH_AP,
    MODE_CONFIGURING,
    MODE_RESET,
    MODE_MANAGER,

    MODE_MAX_VALUE
};

#if defined(APP_DEBUG)
const char *StateStr[MODE_MAX_VALUE + 1] = {
    "SWITCH_STA",
    "RUNNING",
    //"SWITCH_AP",
    "CONFIGURING",
    "RESET",
    "MANAGER",

    "INIT"};
#endif

namespace KurState
{
    volatile State state = MODE_MAX_VALUE;

    State get() { return state; }
    bool is(State m) { return (state == m); }
    void set(State m);
};

void config_init()
{
    Serial.begin(115200);
    pinMode(BOARD_LED_PIN, OUTPUT);
    pinMode(BOARD_BUTTON_PIN, INPUT);
    digitalWrite(BOARD_LED_PIN, LOW);
    ticker.attach(0.5, tick);

    Servo1.attach(SERVO1_PIN, START_DEGREE_CH1);
    Servo2.attach(SERVO2_PIN, START_DEGREE_CH2);
    Servo1.setMinMaxConstraint(SERVO_MIN_LIMIT_DEGREE_CH1, SERVO_MAX_LIMIT_DEGREE_CH1);
    Servo2.setMinMaxConstraint(SERVO_MIN_LIMIT_DEGREE_CH2, SERVO_MAX_LIMIT_DEGREE_CH2);
}

void runotocore()
{
    input = client.read();
    Serial.println(input);
    if (input == 118)
    {

        Servo1.startEaseTo(goc, 375, START_UPDATE_BY_INTERRUPT);

        //Serial.print("ga:__");
        //Serial.print(goc);
        goc = 0;
    }
    if (input == 114)
    {
        Servo2.startEaseTo(goc, 375, START_UPDATE_BY_INTERRUPT);

        //Serial.print("-------------lai:__");
        //Serial.println(goc);
        goc = 0;
    }
    if (input != 118 && input != 114)
    {
        goc = (input - 48) + goc * 10;
        input = 0;
    }
}