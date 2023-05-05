#include "PowerLED.h"

#define POWERLED_TIMER TIM2
#define POWERLED_TIMER_CHN CCR3
#define POWERLED_SET_PWM(n) (POWERLED_TIMER->POWERLED_TIMER_CHN = (n))

static  void PowerLED_ON(void);
static  void PowerLED_OFF(void);

PowerLED_T tPowerLED = 
{
    POWERLED_OFF,
    FALSE,
    PowerLED_ON,
    PowerLED_OFF,
};

static  void PowerLED_ON(void)
{
    if(tPowerLED.Status != POWERLED_ON)
    {
        tPowerLED.Status = POWERLED_ON;
        HAL_GPIO_WritePin(POWER_LED_EN_GPIO_Port, POWER_LED_EN_Pin, GPIO_PIN_SET);
    }
}

static  void PowerLED_OFF(void)
{
    if(tPowerLED.Status != POWERLED_OFF)
    {
        tPowerLED.Status = POWERLED_OFF;
        HAL_GPIO_WritePin(POWER_LED_EN_GPIO_Port, POWER_LED_EN_Pin, GPIO_PIN_RESET);
    }    
}




