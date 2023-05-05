/* Includes ------------------------------------------------------------------*/
#include "SHT30.h"

/* Private define-------------------------------------------------------------*/
#define SHT30_PRIORITY  14
#define SHT30_TIMESLICE 25
/* Private variables----------------------------------------------------------*/
//ALIGN(RT_ALIGN_SIZE)
static char SHT30_ThreadStack[2048];
static struct rt_thread SHT30_Thread;

void Measure_Period_Mode(void);  //���ڲ���ģʽ
static void SHT30_Init(void);
static void SHT30_Thread_Entry(void *paramter);

/* Public variables-----------------------------------------------------------*/
SHT30_t SHT30 =
{
    0,
    0.0,
    SHT30_Init,
    Measure_Period_Mode
};

/* Private function prototypes------------------------------------------------*/
static uint8_t CRC_8(uint8_t*, uint8_t);

static void SHT30_Init(void)
{
    rt_err_t err;
    
    rt_thread_init(&SHT30_Thread,
                    "SHT30",
                    SHT30_Thread_Entry,
                    RT_NULL,
                    &SHT30_ThreadStack[0],
                    sizeof(SHT30_ThreadStack),
                    SHT30_PRIORITY,
                    SHT30_TIMESLICE);
                    
    if(err != RT_ERROR)
    {
        err = rt_thread_startup(&SHT30_Thread);
    }
    
    if(err == RT_ERROR)
        rt_kprintf("SHT30 init fail.\r\n");
}

static void SHT30_Thread_Entry(void *paramter)
{
    while(1)
    {
        Measure_Period_Mode();
//        printf("Wendu = %.1f��\r\n", SHT30.fTemperature);
        rt_kprintf("shidu = %d%%RH\r\n\r\n", (uint16_t)SHT30.ucHumidity);
        rt_thread_mdelay(1000);
    }
}

/*
	* @name   Measure_Period_Mode
	* @brief  ���ڲ���ģʽ
	* @param  None
	* @retval None
*/
static void Measure_Period_Mode(void)
{
    uint8_t   temp_array[6] = {0};
    uint16_t  temp_uint     = 0;
    float     temp_float    = 0;

    //���������Բ���
    I2C_Soft.Start();
    I2C_Soft.Write_Byte(SHT30_ADDR & Write_CMD);
    I2C_Soft.Write_Byte(0x27); //High repeat , mps = 10
    I2C_Soft.Write_Byte(0x37);

    Timer6.SHT30_Measure_Timeout = 0;
    //���ͽ�����������
    do
    {
        if(Timer6.SHT30_Measure_Timeout >= TIMER6_2S) //2s��û��ȡ�����ݣ��˳��ȴ�
            break;

        I2C_Soft.Start();
        I2C_Soft.Write_Byte(SHT30_ADDR & Write_CMD);
        I2C_Soft.Write_Byte(0xE0);
        I2C_Soft.Write_Byte(0x00);

        I2C_Soft.Start();
    }
    while(I2C_Soft.Write_Byte(SHT30_ADDR | Read_CMD) == NACK);

    //��ʼ���ղ������ݣ�������
    if(Timer6.SHT30_Measure_Timeout < TIMER6_2S)
    {
        temp_array[0] = I2C_Soft.Read_Byte(ACK);
        temp_array[1] = I2C_Soft.Read_Byte(ACK);
        temp_array[2] = I2C_Soft.Read_Byte(ACK);
        temp_array[3] = I2C_Soft.Read_Byte(ACK);
        temp_array[4] = I2C_Soft.Read_Byte(ACK);
        temp_array[5] = I2C_Soft.Read_Byte(NACK);
        I2C_Soft.Stop();

        //////�����¶�,����0.1
        if(CRC_8(temp_array, 2) == temp_array[2]) //CRC-8 У��
        {
            temp_uint         = temp_array[0] * 256 + temp_array[1];
            temp_float        = ((float)temp_uint) * 0.267032f - 4500;
            SHT30.fTemperature = temp_float * 0.01f;
        }

        //////����ʪ�ȣ�����1%RH
        if(CRC_8(&temp_array[3], 2) == temp_array[5]) //CRC-8 У��
        {
            temp_uint      = temp_array[3] * 256 + temp_array[4];
            temp_float     = ((float)temp_uint) * 0.152590f;
            temp_float     = temp_float * 0.01f;
            SHT30.ucHumidity = (unsigned char)temp_float;
        }
    }
}

/*
	* @name   CRC_8
	* @brief  CRC-8У��
	* @param  Crc_ptr -> У�������׵�ַ
						LEN     -> У�����ݳ���
	* @retval CRC_Value -> У��ֵ
*/
static uint8_t CRC_8(uint8_t* Crc_ptr, uint8_t LEN)
{
    uint8_t CRC_Value = 0xFF;
    uint8_t i = 0, j = 0;

    for(i = 0; i < LEN; i++)
    {
        CRC_Value ^= *(Crc_ptr + i);
        for(j = 0; j < 8; j++)
        {
            if(CRC_Value & 0x80)
                CRC_Value = (CRC_Value << 1) ^ 0x31;
            else
                CRC_Value = (CRC_Value << 1);
        }
    }
    return CRC_Value;
}

/********************************************************
  End Of File
********************************************************/
