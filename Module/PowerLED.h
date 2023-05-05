#ifndef __POWER_LED_H__
#define __POWER_LED_H__

#include <stdio.h>
#include "tim.h"
#include "gpio.h"
#include "Brightness.h"

typedef enum
{
    POWERLED_OFF = 0,
    POWERLED_ON,
}PowerLED_Status_E;

typedef enum
{
    BRIGHT_NONE = 0,
    BRIGHT_H = 1000u,
    BRIGHT_M = 250u,
    BRIGHT_L = 100u,
}PowerLED_Bright_E;

typedef struct
{
    uint8_t Status;
    uint8_t AutoAdjustFlag;
    void (*ON)(void);
    void (*OFF)(void);
    
}PowerLED_T;

extern PowerLED_T tPowerLED;

#endif
