#ifndef __I2C_H___
#define __I2C_H___

#include "main.h"
#include "tim.h"

//宏定义
//定义枚举类型
typedef enum
{
	ACK	 = GPIO_PIN_RESET,
	NACK = GPIO_PIN_SET,
}ACK_Value_t;

//定义结构体类型
typedef struct
{
	void (*Init)(void);  //I2C初始化
	void (*Start)(void); //I2C起始信号
	void (*Stop)(void);  //I2C停止信号
	ACK_Value_t (*Write_Byte)(uint8_t);      //I2C写字节
	uint8_t     (*Read_Byte) (ACK_Value_t);  //I2C读字节
}I2C_Soft_t;

/* extern variables-----------------------------------------------------------*/
extern I2C_Soft_t  I2C_Soft;

/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
