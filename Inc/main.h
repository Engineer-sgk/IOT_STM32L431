/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_ALARM_Pin GPIO_PIN_5
#define LED_ALARM_GPIO_Port GPIOA
#define LED_RUN_Pin GPIO_PIN_6
#define LED_RUN_GPIO_Port GPIOA
#define LED_WIFI_Pin GPIO_PIN_7
#define LED_WIFI_GPIO_Port GPIOA
#define POWER_LED_EN_Pin GPIO_PIN_0
#define POWER_LED_EN_GPIO_Port GPIOB
#define RELAY_Pin GPIO_PIN_1
#define RELAY_GPIO_Port GPIOB
#define WIFI_RST_Pin GPIO_PIN_9
#define WIFI_RST_GPIO_Port GPIOC
#define WIFI_EN_Pin GPIO_PIN_8
#define WIFI_EN_GPIO_Port GPIOA
#define SHT30_SCL_Pin GPIO_PIN_6
#define SHT30_SCL_GPIO_Port GPIOB
#define SHT30_SDA_Pin GPIO_PIN_7
#define SHT30_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
//定义枚举类型 -> BIT位
typedef enum
{
	BIT0 = (uint8_t)(0x01 << 0),  
	BIT1 = (uint8_t)(0x01 << 1),  
	BIT2 = (uint8_t)(0x01 << 2),  
	BIT3 = (uint8_t)(0x01 << 3),  
	BIT4 = (uint8_t)(0x01 << 4),
	BIT5 = (uint8_t)(0x01 << 5),
	BIT6 = (uint8_t)(0x01 << 6),
	BIT7 = (uint8_t)(0x01 << 7),
}BIT_t;

typedef enum 
{
  FALSE = 0u, 
  TRUE = !FALSE
} FlagStatus_t;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
