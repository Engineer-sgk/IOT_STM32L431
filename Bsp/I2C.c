/* Includes ------------------------------------------------------------------*/
#include "I2C.h"

/* Private define-------------------------------------------------------------*/
//置位与清零SCL管脚
#define	SET_SCL	HAL_GPIO_WritePin(SHT30_SCL_GPIO_Port,SHT30_SCL_Pin,GPIO_PIN_SET)
#define	CLR_SCL	HAL_GPIO_WritePin(SHT30_SCL_GPIO_Port,SHT30_SCL_Pin,GPIO_PIN_RESET)
//置位与清零SDA管脚
#define	SET_SDA	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port,SHT30_SDA_Pin,GPIO_PIN_SET)
#define	CLR_SDA	HAL_GPIO_WritePin(SHT30_SDA_GPIO_Port,SHT30_SDA_Pin,GPIO_PIN_RESET)
//读SDA管脚状态
#define READ_SDA	HAL_GPIO_ReadPin(SHT30_SDA_GPIO_Port,SHT30_SDA_Pin)

/* Private variables----------------------------------------------------------*/
void Init(void);  //I2C初始化
void Start(void); //I2C起始信号
void Stop(void);  //I2C停止信号
ACK_Value_t Write_Byte(uint8_t);      //I2C写字节
uint8_t     Read_Byte(ACK_Value_t);   //I2C读字节

/* Public variables-----------------------------------------------------------*/
I2C_Soft_t I2C_Soft =
{
    Init,
    Start,
    Stop,
    Write_Byte,
    Read_Byte
};

/* Private function prototypes------------------------------------------------*/


/*
	* @name   Init
	* @brief  I2C初始化
	* @param  None
	* @retval None
*/
static void Init(void)
{
    SET_SCL;
    SET_SDA;
}

/*
	* @name   Start
	* @brief  I2C起始信号
	* @param  None
	* @retval None
*/
static void Start(void)
{
    //SCL为高电平，SDA的下降沿为I2C起始信号
    SET_SDA;
    SET_SCL;
    Delay_us(1);

    CLR_SDA;
    Delay_us(10);

    CLR_SCL;
    Delay_us(1);
}

/*
	* @name   Stop
	* @brief  I2C停止信号
	* @param  None
	* @retval None
*/
static void Stop(void)
{
    //SCL为高电平，SDA的上升沿为I2C停止信号
    CLR_SDA;
    SET_SCL;
    Delay_us(1);

    Delay_us(10);
    SET_SDA;
}

/*
	* @name   Write_Byte
	* @brief  I2C写字节
	* @param  WR_Byte -> 待写入数据
	* @retval ACK_Value_t -> 从机应答值
*/
static ACK_Value_t Write_Byte(uint8_t WR_Byte)
{
    uint8_t i;
    ACK_Value_t  ACK_Rspond;

    //SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
    //数据按8位传输，高位在前，利用for循环逐个接收
    for(i = 0; i < 8; i++)
    {
        //SCL清零，主机SDA准备数据
        CLR_SCL;
        Delay_us(1);
        if((WR_Byte & BIT7) == BIT7)
        {
            SET_SDA;
        }
        else
        {
            CLR_SDA;
        }
        Delay_us(1);
        //SCL置高，传输数据
        SET_SCL;
        Delay_us(10);

        //准备发送下一比特位
        WR_Byte <<= 1;
    }

    CLR_SCL;
    //释放SDA，等待从机应答
    SET_SDA;
    Delay_us(1);

    SET_SCL;
    Delay_us(10);

    ACK_Rspond = (ACK_Value_t)READ_SDA;

    CLR_SCL;
    Delay_us(1);

    //返回从机的应答信号
    return ACK_Rspond;
}

/*
	* @name   Write_Byte
	* @brief  I2C写字节
	* @param  ACK_Value -> 主机回应值
	* @retval 从机返回值
*/
static uint8_t Read_Byte(ACK_Value_t ACK_Value)
{
    uint8_t RD_Byte = 0, i;

    ////接收数据
    //SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
    //数据按8位传输，高位在前，利用for循环逐个接收
    for(i = 0; i < 8; i++)
    {
        //准备接收下一比特位
        RD_Byte <<= 1;

        //SCL清零，从机SDA准备数据
        CLR_SCL;
        Delay_us(10);

        //SCL置高，获取数据
        SET_SCL;
        Delay_us(10);

        RD_Byte |= READ_SDA;
    }


    //SCL清零，主机准备应答信号
    CLR_SCL;
    Delay_us(1);

    //主机发送应答信号
    if(ACK_Value == ACK)
    {
        CLR_SDA;
    }
    else
    {
        SET_SDA;
    }
    Delay_us(1);


    SET_SCL;
    Delay_us(10);

    //Note:
    //释放SDA数据线
    //SCL先清零，再释放SDA，防止连续传输数据时，从机错将SDA释放信号当成NACk信号
    CLR_SCL;
    SET_SDA;
    Delay_us(1);

    //返回数据
    return RD_Byte;
}

/********************************************************
  End Of File
********************************************************/
