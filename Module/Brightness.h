#ifndef __BRIGHTNESS_H__
#define __BRIGHTNESS_H__

#include <stdio.h>
#include "adc.h"
#include <rtthread.h>

typedef struct
{
    uint8_t eResult;
    void (*Init)(void);
    void (*Detect)(void);
}Brightness_T;

extern Brightness_T tBrightness;

#endif
