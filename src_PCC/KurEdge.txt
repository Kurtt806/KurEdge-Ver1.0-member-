extern "C"
{
#include "user_interface.h"
    void app_loop();
    void restartMCU();
    void tick();
}

#include "KurSettings.h"
#include "KurStore.h"
#include "KurMode.h"

inline void KurState::set(State m)
{
    if (state != m && m < MODE_MAX_VALUE)
    {
        String(StateStr[state]) + " => " + StateStr[m];
        state = m;
    }
}

void app_loop()
{
#if defined(KURCORES)
    if (CLIENT_ALIVE == true)
    {
        if (millis() - aliveSentTime > 2000)
        {
            CLIENT_DIE = true;
        }
        else
        {
            CLIENT_DIE = false;
        }
    }
    else
    {
        aliveSentTime = millis();
    }
#endif
}

class KurEdge
{
public:
    void begin(void)
    {
        config_init();
    }
    void run(void)
    {
        app_loop();
        switch (KurState::get())
        {
        case MODE_SWITCH_STA:
            enterStaSwitch();
            break;
        case MODE_RUNNING:
            enterRunWithChecks();
            break;
        case MODE_SWITCH_AP:
            enterApSwitch();
            break;
        case MODE_CONFIGURING:
            enterConfigMode();
            break;
        case MODE_RESET:
            enterReset();
            break;
        case MODE_MANAGER:
            enterManagerMode();
            break;
        case MODE_OTO_CODE:
            enterOTOCODE();
            break;
        default:
            enterManagerMode();
            break;
        }
    }
} KurEdge;
