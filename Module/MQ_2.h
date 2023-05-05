#ifndef __MQ_2_H__
#define __MQ_2_H__

#include "adc.h"
#include "usart.h"
#include <rtthread.h>

typedef struct
{
    uint8_t AQI;
    void (*Init)(void);
    void (*Detect)(void);
}MQ_2_T;

extern MQ_2_T tMQ_2;


#endif
