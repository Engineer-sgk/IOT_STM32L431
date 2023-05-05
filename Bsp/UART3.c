/* Includes ------------------------------------------------------------------*/
#include "UART3.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

static uint8_t  ucSend_Buffer[UART3_Send_LENGTH] = {0x00};
static uint8_t  ucRec_Buffer [UART3_Rec_LENGTH]  = {0x00};

/* Private function prototypes------------------------------------------------*/
static void SendArray(uint8_t*, uint16_t); //串口发送数组
static void SendString(uint8_t*);          //串口发送字符串

static void RS485_Set_SendMode(void); //RS-485接口设置为发送模式
static void RS485_Set_RecMode(void);  //RS-485接口设置为接收模式

/* Public variables-----------------------------------------------------------*/
UART_t  UART3 =
{
    ucSend_Buffer,
    ucRec_Buffer,

    SendArray,
    SendString,

    TTL,
    RS485_Set_SendMode,
    RS485_Set_RecMode
};

/*
	* @name   SendArray
	* @brief  串口发送数组
	* @param  p_Arr:数组首地址，LEN:发送长度
	* @retval None
*/
static void SendArray(uint8_t* p_Arr, uint16_t LEN)
{
    HAL_UART_Transmit_DMA(&huart3, p_Arr, LEN);
}


/*
	* @name   SendString
	* @brief  发送字符串
	* @param  p_Str:待发送字符串
	* @retval None
*/
static void SendString(uint8_t* p_Str)
{
    HAL_UART_Transmit_DMA(&huart3, p_Str, strlen((const char*)p_Str));
}

/*
	* @name   RS485_Set_SendMode
	* @brief  RS-485接口设置为发送模式
	* @param  None
	* @retval None
*/
static void RS485_Set_SendMode()
{

}

/*
	* @name   RS485_Set_RecMode
	* @brief  RS-485接口设置为接收模式
	* @param  None
	* @retval None
*/
static void RS485_Set_RecMode()
{

}
/********************************************************
  End Of File
********************************************************/
