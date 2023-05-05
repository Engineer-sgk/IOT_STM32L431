#include "Relay.h"

static void Relay_ON(void);
static void Relay_OFF(void);

Relay_T tRelay = 
{
    RELAY_OFF,
    Relay_ON,
    Relay_OFF,
};

static void Relay_ON(void)
{
    if(tRelay.eFlag != RELAY_ON)
    {
        tRelay.eFlag = RELAY_ON;
        HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
    }
}

static void Relay_OFF(void)
{
    if(tRelay.eFlag != RELAY_OFF)
    {
        tRelay.eFlag = RELAY_OFF;
        HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
    }
}

