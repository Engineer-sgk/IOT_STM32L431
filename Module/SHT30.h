#ifndef __SHT30_H__
#define __SHT30_H__


#include "main.h"
#include "I2C.h"
#include "Timer6.h"
#include <rtthread.h>

//宏定义
#define SHT30_ADDR  (uint8_t)(0x44 << 1) //传感器地址
//#define SHT30_ADDR  (unsigned char)(0x45 << 1) //传感器地址

#define	Write_CMD   0xFE
#define	Read_CMD    0x01

//定义枚举类型

//定义结构体类型

typedef struct
{
    uint8_t ucHumidity;    //湿度 0%RH至100%RH  精度1%RH           
	float   fTemperature;  //温度 -40至125℃    精度0.1℃ 
	
    void (*Init)(void);
	void (*Measure_Period_Mode)(void);  //周期测量模式
}SHT30_t;


/* extern variables-----------------------------------------------------------*/
extern SHT30_t  SHT30;

/* ADDR Pin Conect to VSS */

//#include "i2c.h"

//#define	SHT30_ADDR_WRITE	0x44<<1         //10001000
//#define	SHT30_ADDR_READ		(0x44<<1)+1	    //10001011

//typedef enum
//{
//    /* 软件复位命令 */

//    SOFT_RESET_CMD = 0x30A2,	
//    /*
//    单次测量模式
//    命名格式：Repeatability_CS_CMD
//    CS： Clock stretching
//    */
//    HIGH_ENABLED_CMD    = 0x2C06,
//    MEDIUM_ENABLED_CMD  = 0x2C0D,
//    LOW_ENABLED_CMD     = 0x2C10,
//    HIGH_DISABLED_CMD   = 0x2400,
//    MEDIUM_DISABLED_CMD = 0x240B,
//    LOW_DISABLED_CMD    = 0x2416,

//    /*
//    周期测量模式
//    命名格式：Repeatability_MPS_CMD
//    MPS：measurement per second
//    */
//    HIGH_0_5_CMD   = 0x2032,
//    MEDIUM_0_5_CMD = 0x2024,
//    LOW_0_5_CMD    = 0x202F,
//    HIGH_1_CMD     = 0x2130,
//    MEDIUM_1_CMD   = 0x2126,
//    LOW_1_CMD      = 0x212D,
//    HIGH_2_CMD     = 0x2236,
//    MEDIUM_2_CMD   = 0x2220,
//    LOW_2_CMD      = 0x222B,
//    HIGH_4_CMD     = 0x2334,
//    MEDIUM_4_CMD   = 0x2322,
//    LOW_4_CMD      = 0x2329,
//    HIGH_10_CMD    = 0x2737,
//    MEDIUM_10_CMD  = 0x2721,
//    LOW_10_CMD     = 0x272A,
//	/* 周期测量模式读取数据命令 */
//	READOUT_FOR_PERIODIC_MODE = 0xE000,
//} SHT30_CMD;

//uint8_t SHT30_Init(void);
//void SHT30_Reset(void);
//uint8_t SHT30_Read_Dat(uint8_t* dat);
//uint8_t SHT30_Dat_To_Float(uint8_t* const dat, float* temperature, float* humidity);

#endif
/********************************************************
  End Of File
********************************************************/
