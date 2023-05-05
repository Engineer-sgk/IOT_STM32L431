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

//�궨��
#define LED_WIFI_ON HAL_GPIO_WritePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin, GPIO_PIN_RESET)
#define LED_WIFI_OFF HAL_GPIO_WritePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin, GPIO_PIN_SET)
#define LED_WIFI_TOGGLE HAL_GPIO_TogglePin(LED_WIFI_GPIO_Port, LED_WIFI_Pin)
//����ö������

//����ṹ������
typedef struct
{
    uint8_t TCP_Connect_Status;            //TCP����״̬
    uint16_t TCP_Reconnect_Timer;          //TCP������ʱ��
    uint8_t  SmartConifg_Flag;             //����������־λ

    void (*Init)(void);                    //ESP8266��ʼ��
    void (*SmartConifg)(void);             //WIFIģ������
    void (*TCP_Connect_Server)(void);      //ͨ��TCP���ӷ�����
    void (*Transfer_SHT30)(void);          //����SHT30����ʪ��
    void (*Receive_Information)(void);     //������Ϣ
    void (*DMA_Receive_Set)(void);         //DMA��������
    void (*Error)(void);                   //������
} ESP8266_t;

/* extern variables-----------------------------------------------------------*/

extern ESP8266_t ESP8266;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
