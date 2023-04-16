
#if defined(MANAGER_WIFI_NAME) && !defined(MANAGER_WIFI_PASS)
MANAGER_WIFI_PASS = NULL;
#endif

#if !defined(MANAGER_WIFI_NAME)
#error "Please specify your MANAGER_WIFI_NAME and MANAGER_WIFI_PASS"
#endif

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "ServoEasing.hpp"
#include <Ticker.h>

const char SSID[] = MANAGER_WIFI_NAME;
const char PASS[] = MANAGER_WIFI_PASS;
IPAddress _ip = IPAddress(192, 168, 1, 11);
IPAddress _gw = IPAddress(192, 168, 1, 1);
IPAddress _sn = IPAddress(255, 255, 255, 0);

WiFiServer serverRobo(80);

WiFiClient client;

bool RESTARTWIFI = false;
bool CLIENT_ALIVE = false;
bool CLIENT_DIE = false;

Ticker ticker;
ServoEasing Servo1;
ServoEasing Servo2;

enum State
{
    MODE_SWITCH_STA,
    MODE_RUNNING,
    MODE_SWITCH_AP,
    MODE_CONFIGURING,
    MODE_RESET,
    MODE_MANAGER,

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
    ticker.attach(0.5, tick);
    digitalWrite(BOARD_LED_PIN, LOW);
    Servo1.attach(SERVO1_PIN, START_DEGREE_CH1);
    Servo2.attach(SERVO2_PIN, START_DEGREE_CH2);
    Servo1.setMinMaxConstraint(SERVO_MIN_LIMIT_DEGREE_CH1, SERVO_MAX_LIMIT_DEGREE_CH1);
    Servo2.setMinMaxConstraint(SERVO_MIN_LIMIT_DEGREE_CH2, SERVO_MAX_LIMIT_DEGREE_CH2);
}

// *------------------------------------------------------------------

long timeSinceREFRESH = 0;
char cmd[100];
int cmdIndex;
unsigned long lastCmdTime = 0;
unsigned long aliveSentTime = 0;

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
/*-------PROGRAM--------------------------------------------------*/

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
            Serial.print(" - content: ");
            int data = (int)atof(cmd + 4);

            Servo1.startEaseTo(data, 375, START_UPDATE_BY_INTERRUPT);
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
            Serial.print(" - content: ");
            int data = (int)atof(cmd + 4);

            Servo2.startEaseTo(data, 375, START_UPDATE_BY_INTERRUPT);
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