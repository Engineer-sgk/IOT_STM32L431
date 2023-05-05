/* Includes ------------------------------------------------------------------*/
#include "Timer6.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      
static void Timer6_Start_IT(void);  //定时器6以中断模式启动
	
/* Public variables-----------------------------------------------------------*/
Timer6_t  Timer6 = 
{
	0,
	0,
	0,
    
	Timer6_Start_IT     
};

/*
	* @name   Timer6_Start_IT
	* @brief  定时器6以中断模式启动
	* @param  None
	* @retval None      
*/
static void Timer6_Start_IT(void)
{
	HAL_TIM_Base_Start_IT(&htim6); //启动定时器6
}
/********************************************************
  End Of File
********************************************************/
