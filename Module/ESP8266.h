#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "UART3.h"
#include "Timer6.h"
#include "SHT30.h"
#include "PowerLED.h"
#include "Relay.h"
#include "FAN.h"
#include "Brightness.h"
#include "MQ_2.h"

//宏定义
#define LED_WIFI_ON HAL_GPIO_WritePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin, GPIO_PIN_RESET)
#define LED_WIFI_OFF HAL_GPIO_WritePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin, GPIO_PIN_SET)
#define LED_WIFI_TOGGLE HAL_GPIO_TogglePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin)
//定义枚举类型

//定义结构体类型
typedef struct
{
    uint8_t TCP_Connect_Status;            //TCP连接状态
    uint16_t TCP_Reconnect_Timer;          //TCP重连定时器
    uint8_t  SmartConifg_Flag;             //智能配网标志位

    void (*Init)(void);                    //ESP8266初始化
    void (*SmartConifg)(void);             //WIFI模块配网
    void (*TCP_Connect_Server)(void);      //通过TCP连接服务器
    void (*Transfer_SHT30)(void);          //传送SHT30的温湿度
    void (*Receive_Information)(void);     //接收信息
    void (*DMA_Receive_Set)(void);         //DMA接收设置
    void (*Error)(void);                   //错误处理
} ESP8266_t;

/* extern variables-----------------------------------------------------------*/

extern ESP8266_t ESP8266;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
