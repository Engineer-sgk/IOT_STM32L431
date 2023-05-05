#ifndef __TIMER6_H__
#define __TIMER6_H__

#include "tim.h"

//定义枚举类型
typedef enum
{
    TIMER6_10mS  	= (uint16_t)2,
    TIMER6_50mS  	= (uint16_t)10,
    TIMER6_100mS	= (uint16_t)20,
    TIMER6_200mS	= (uint16_t)40,
    TIMER6_500mS	= (uint16_t)100,
    TIMER6_1S     = (uint16_t)200,
    TIMER6_2S     = (uint16_t)400,
    TIMER6_3S     = (uint16_t)600,
    TIMER6_5S     = (uint16_t)1000,
    TIMER6_10S    = (uint16_t)2000,
    TIMER6_3min   = (uint16_t)36000,
} TIMER6_Value_t;

//定义结构体类型
typedef struct
{
    uint16_t volatile usMCU_Run_Timer;  //系统运行定时器
    uint16_t volatile usDelay_Timer;    //延时定时器
    uint16_t volatile SHT30_Measure_Timeout;

    void (*Timer6_Start_IT)(void);      //定时器6以中断模式启动
} Timer6_t;

/* extern variables-----------------------------------------------------------*/
extern Timer6_t  Timer6;

#endif
/********************************************************
  End Of File
********************************************************/
