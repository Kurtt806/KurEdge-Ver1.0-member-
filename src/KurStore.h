
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
#include <Servo.h>
Servo ServoGa, ServoLai;
int goc = 0, input = 0;

#endif

#if defined(KURCORES)
#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define DISABLE_COMPLEX_FUNCTIONS
#define MAX_EASING_SERVOS 2
#define DEBUG 
#include "ServoEasing.hpp"
ServoEasing Servo1;
ServoEasing Servo2;
long timeSinceREFRESH = 0;
char cmd[100];
int cmdIndex;
unsigned long lastCmdTime = 0;
unsigned long aliveSentTime = 0;
bool RESTARTWIFI = false;
bool CLIENT_ALIVE = false;
bool CLIENT_DIE = false;
#endif

#include <Ticker.h>
Ticker ticker;
WiFiServer serverRobo(80);
WiFiClient client;

enum State
{
    MODE_SWITCH_STA,
    MODE_RUNNING,
    MODE_SWITCH_AP,
    MODE_CONFIGURING,
    MODE_RESET,
    MODE_MANAGER,
    MODE_OTO_CODE,

    MODE_MAX_VALUE
};

#if defined(APP_DEBUG)
const char *StateStr[MODE_MAX_VALUE + 1] = {
    "SWITCH_STA",
    "RUNNING",
    "SWITCH_AP",
    "CONFIGURING",
    "RESET",
    "MANAGER",
    "OTO",

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

#if defined(OTOCORES)
    ServoGa.attach(SERVO2_PIN);
    ServoLai.attach(SERVO1_PIN);
#endif
#if defined(KURCORES)
    Servo1.attach(SERVO1_PIN, START_DEGREE_CH1);
    Servo2.attach(SERVO2_PIN, START_DEGREE_CH2);
    // Servo1.setMinMaxConstraint(SERVO_MIN_LIMIT_DEGREE_CH1, SERVO_MAX_LIMIT_DEGREE_CH1);
    // Servo2.setMinMaxConstraint(SERVO_MIN_LIMIT_DEGREE_CH2, SERVO_MAX_LIMIT_DEGREE_CH2);

#endif
}

#if defined(OTOCORES)
void runotocore()
{
    input = client.read();
    if (input == 118)
    {
        ServoGa.write(goc);
        Serial.print("ga:__");
        Serial.print(goc);
        goc = 0;
    }
    if (input == 114)
    {
        ServoLai.write(goc);
        Serial.print("-------------lai:__");
        Serial.println(goc);
        goc = 0;
    }
    if (input != 118 && input != 114)
    {
        goc = (input - 48) + goc * 10;
        input = 0;
    }
}
#endif

#if defined(KURCORES)
/*-------PROGRAM_kurDEV (begin)------------------------------------------*/
boolean cmdStartsWith(const char *st)
{
    for (int i = 0;; i++)
    {
        if (st[i] == 0)
            return true;
        if (cmd[i] == 0)
            return false;
        if (cmd[i] != st[i])
            return false;
        ;
    }
    return false;
}
void thuchienlenh()
{
    lastCmdTime = millis();
    if (cmdStartsWith("CH1"))
    {
        int ch = cmd[2] - '0';
        if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
        {
            Serial.print(" - kenh: ");
            Serial.print(ch);
            Serial.print(" - ");
            int data = (int)atof(cmd + 4);

             Servo1.startEaseTo(data, SERVO_SPEED_DEGREE_CH1, START_UPDATE_BY_INTERRUPT);
            //analogWrite(SERVO1_PIN, data);
            Serial.println(data);
        }
    }
    if (cmdStartsWith("CH2"))
    {
        int ch = cmd[2] - '0';
        if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
        {
            Serial.print(" - kenh: ");
            Serial.print(ch);
            Serial.print(" - ");
            int data = (int)atof(cmd + 4);

             Servo2.startEaseTo(data, SERVO_SPEED_DEGREE_CH2, START_UPDATE_BY_INTERRUPT);
            //analogWrite(SERVO2_PIN, data);

            Serial.println(data);
        }
    }

    if (cmdStartsWith("reset"))
    {
        delay(100);
        ESP.restart();
    }
    if (cmdStartsWith("alive"))
    {
        aliveSentTime = millis();
    }
    if (cmdStartsWith("manager"))
    {
        Serial.println("resetsetting");
        RESTARTWIFI = true;
    }
}
void Phone_ESP()
{
    char c = (char)client.read();
    if (c == '\n')
    {
        cmd[cmdIndex] = 0;
        thuchienlenh();
        cmdIndex = 0;
    }
    else
    {
        cmd[cmdIndex] = c;
        if (cmdIndex < 10)
            cmdIndex++;
    }
}
void ESP_Phone()
{
    int ssi = WiFi.RSSI();
    char ssi2[4];
    itoa(ssi, ssi2, 10);
    client.write("x ");
    client.write(ssi2);
    client.write("\n");
    client.write("ON on\n");
}
/*-------PROGRAM_kurDEV (end)------------------------------------------*/
#endif