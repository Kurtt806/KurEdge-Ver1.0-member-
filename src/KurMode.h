

void restartMCU()
{
    ESP.restart();
    ESP.reset();
    while (1)
    {
    };
}

void tick()
{
    (digitalRead(BOARD_LED_PIN) == LOW) ? digitalWrite(BOARD_LED_PIN, HIGH) : digitalWrite(BOARD_LED_PIN, LOW);
}

void enterManagerMode()
{
    KurState::set(MODE_MANAGER);
    Serial.println("---this is MODE_MANAGER---");
    //*---------------------------------------------
    ticker.attach(0.5, tick);
    CLIENT_ALIVE = false;
    RESTARTWIFI = false;
    WiFi.mode(WIFI_OFF);
    delay(3000);
    WiFiManager wifiManager;
    // wifiManager.setBreakAfterConfig(true);
    wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
    if (digitalRead(BOARD_BUTTON_PIN) == BOARD_BUTTON_ACTIVE)
    {
        ticker.attach(0.4, tick);
        if (!wifiManager.autoConnect(SSID, PASS))
        {
            Serial.println("---Wifi not connected---");
            delay(2000);
            KurState::set(MODE_RESET);
        }
        KurState::set(MODE_SWITCH_STA);
    }
    else
    {
        ticker.attach(0.1, tick);
        KurState::set(MODE_CONFIGURING);
    }
}

void enterStaSwitch()
{
    KurState::set(MODE_SWITCH_STA);
    Serial.println("---this is MODE_SWITCH_STA---");
    //*---------------------------------------------
    ticker.attach(1.5, tick);
    WiFi.mode(WIFI_STA);
    Serial.print("---SSID: ");
    Serial.println(WiFi.SSID());

    serverRobo.begin();

    KurState::set(MODE_RUNNING);
}

void enterRunWithChecks()
{
    KurState::set(MODE_RUNNING);
    // Serial.println("---this is MODE_RUNNING---");
    //*---------------------------------------------
    switch (client.connected())
    {
    case true:
        ticker.detach();
        digitalWrite(BOARD_LED_PIN, LOW);
        if (client.available())
        {
            Phone_ESP();
            CLIENT_ALIVE = true;
        }
        break;
    case false:
        client = serverRobo.available();
        break;
    default:
        break;
    }
    if (CLIENT_ALIVE == true && CLIENT_DIE == true)
    {
        KurState::set(MODE_MANAGER);
    }
    if (millis() - timeSinceREFRESH > REFRESH_DURATION)
    {
        // -> data transmit
        ESP_Phone();
        timeSinceREFRESH = millis();
    }
    if (RESTARTWIFI == true)
    {
        KurState::set(MODE_RESET);
    }
    KurState::is(MODE_RUNNING);
}

void enterApSwitch()
{
    KurState::set(MODE_SWITCH_AP);
    Serial.println("---this is MODE_SWITCH_AP---");
    //*---------------------------------------------

    KurState::set(MODE_CONFIGURING);
}

void enterConfigMode()
{
    KurState::set(MODE_CONFIGURING);
    Serial.println("---this is MODE_CONFIGURING---");
    //*---------------------------------------------
    ticker.attach(0.1, tick);
    WiFiManager wifiManager;
    wifiManager.startConfigPortal(SSID, PASS);

    KurState::set(MODE_MANAGER);
}

void enterReset()
{
    KurState::set(MODE_RESET);
    Serial.println("---this is MODE_RESET---");
    //*---------------------------------------------
    // delay(1000);
    /*
    unsigned long timeoutMs = millis() + 5000;
    while (timeoutMs > millis())
    {
        // all control go to home
        delay(10);
        app_loop();
        if (!KurState::is(MODE_RESET))
        {
            return;
        }
    }
    */
    restartMCU();
}
