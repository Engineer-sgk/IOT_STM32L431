#ifndef __FAN_H__
#define __FAN_H__

#include "tim.h"
#include <rtthread.h>

typedef enum
{
    FAN_SPEED_H = 1000u,
    FAN_SPEED_M = 500u,
    FAN_SPEED_L = 100u,
}FAN_SPEEDE_E;

typedef enum
{
    FAN_STA_OFF = 0u,
    FAN_STA_ON,
}FAN_STATUS_E;

typedef struct
{
    FAN_STATUS_E eFlag;
    void (*Init)(void);
    void (*ON)(void);
    void (*OFF)(void);
    void (*AdjustSpeed)(uint16_t );
}FAN_T;


extern FAN_T tFAN;






#endif
