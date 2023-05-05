#include "FAN.h"

#define FAN_PWM_OF_TIMER TIM2
#define FAN_SET_SPEED(x) (FAN_PWM_OF_TIMER->CCR1 = (x))

static void FAN_Init(void);
static void FAN_ON(void);
static void FAN_OFF(void);
static void AdjustSpeed(uint16_t uiSpeed);

FAN_T tFAN = 
{
    FAN_STA_OFF,
    FAN_Init,
    FAN_ON,
    FAN_OFF,
    AdjustSpeed,
};

static void FAN_Init(void)
{
    
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

    
}

static void FAN_ON(void)
{
    if(tFAN.eFlag != FAN_STA_ON)
    {
        tFAN.eFlag = FAN_STA_ON;
        FAN_SET_SPEED(FAN_SPEED_L);
    }
}

static void FAN_OFF(void)
{
    if(tFAN.eFlag != FAN_STA_OFF)
    {
        tFAN.eFlag = FAN_STA_OFF;
        FAN_SET_SPEED(FAN_STA_OFF);
    }
}

static void AdjustSpeed(uint16_t uiSpeed)
{
    if(tFAN.eFlag == FAN_STA_ON)
    {
        FAN_SET_SPEED(uiSpeed);
    }
}






