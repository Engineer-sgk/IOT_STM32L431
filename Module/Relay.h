#ifndef __RELAY_H__
#define __RELAY_H__

#include "gpio.h"

typedef enum
{
    RELAY_OFF = 0,
    RELAY_ON,
}RELAY_STATUS_E;

typedef struct
{
    RELAY_STATUS_E eFlag;
    void (*ON)(void);
    void (*OFF)(void);
}Relay_T;

extern Relay_T tRelay;

#endif
