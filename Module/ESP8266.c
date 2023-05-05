/* Includes ------------------------------------------------------------------*/
#include "ESP8266.h"

/* Private define-------------------------------------------------------------*/
//#define TCP_Server  (uint8_t*)"AT+CIPSTART=\"TCP\",\"192.168.31.19\",8888\r\n"  //电脑端模拟的TCP服务器
#define TCP_Server  (uint8_t*)"AT+CIPSTART=\"TCP\",\"112.74.142.132\",8647\r\n"  //电脑端模拟的TCP服务器
#define SERIAL_NUM (uint8_t *)"97783ANHE3SEK8U5\r\n"
//#define TCP_Server  (uint8_t*)"AT+CIPSTART=\"TCP\",\"192.168.1.132\",8888\r\n"  //手机端模拟的TCP服务器

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/
static void Init(void);                            //ESP8266初始化
static void SmartConifg(void);                     //WIFI模块配网
static void TCP_Connect_Server(void);              //通过TCP连接服务器
static void Transfer_SHT30(void);                  //传送SHT30的温湿度
static void Receive_Information(void);             //接收信息
static void DMA_Receive_Set(void);                 //DMA重新设置
static void Error(void);                           //错误信息

static void SendAT(uint8_t*, uint8_t*);            //发送AT指令

/* Public variables-----------------------------------------------------------*/
ESP8266_t  ESP8266 =
{
    FALSE,
    TIMER6_10S,
    FALSE,

    Init,
    SmartConifg,
    TCP_Connect_Server,
    Transfer_SHT30,
    Receive_Information,
    DMA_Receive_Set,
    Error
};

/*
	* @name   Init
	* @brief  ESP8266初始化
	* @param  None
	* @retval None
*/
static void Init()
{
    //复位模组
    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);

    //使能模组
    HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET);
    //延时500ms，等待WIFI模块稳定，准备好接收AT指令
    HAL_Delay(500);

    //同步波特率
    Timer6.usDelay_Timer = 0;
    do
    {
        //DMA重新接收设置
        ESP8266.DMA_Receive_Set();

        //发送AT指令
        UART3.SendString((uint8_t*)"AT\r\n");
        //延时100ms，等待接收完成
        HAL_Delay(100);

        //打印信息
        printf("%s", UART3.pucRec_Buffer);

        //超时处理
        if(Timer6.usDelay_Timer >= TIMER6_10S)
        {
            ESP8266.Error();
            break;
        }
    }
    while(strstr((const char*)UART3.pucRec_Buffer, "OK") == NULL);
}

/*
	* @name   SendAT
	* @brief  发送AT指令
	* @param  AT_Command  -> 待发送的AT指令
	          Respond_Str -> 回应数据中包含的字符串
	* @retval None
*/
static void SendAT(uint8_t* AT_Command, uint8_t* Respond_Str)
{
    uint8_t* const  Ptr_AT_Command   = AT_Command;
    uint8_t* const  Ptr_Respond_Str  = Respond_Str;

    //DMA重新接收设置
    ESP8266.DMA_Receive_Set();

    //发送AT指令
    UART3.SendString(Ptr_AT_Command);
    //打印信息
    printf("%s", Ptr_AT_Command);

    //等待模块回应数据，超时错误处理
    Timer6.usDelay_Timer = 0;
    while(strstr((const char*)UART3.pucRec_Buffer, "\r\n") == NULL)
    {
        if(Timer6.usDelay_Timer > TIMER6_100mS)
        {
            ESP8266.Error();
            break;
        }
    }
    //延时10ms，接收完全部字节
    HAL_Delay(10);

    //模块回应数据处理处理
    if(strstr((const char*)UART3.pucRec_Buffer, (const char*)Ptr_Respond_Str) == NULL)
    {
        ESP8266.Error();
    }

    //打印信息
    printf("%s", UART3.pucRec_Buffer);
}

/*
	* @name   SmartConifg
	* @brief  WIFI模块配网
	* @param  None
	* @retval None
*/
static void SmartConifg()
{
    if(ESP8266.SmartConifg_Flag == TRUE)
    {
        //关闭指示灯

        //退出透传模式
        *(UART3.pucSend_Buffer + 0) = '+';
        *(UART3.pucSend_Buffer + 1) = '+';
        *(UART3.pucSend_Buffer + 2) = '+';
        UART3.SendArray(UART3.pucSend_Buffer, 3);
        HAL_Delay(1000);

        //配网
        SendAT((uint8_t*)"AT\r\n", (uint8_t*)"OK");               //测试AT
        SendAT((uint8_t*)"ATE0\r\n", (uint8_t*)"OK");             //关闭回显
        SendAT((uint8_t*)"AT+CWMODE_CUR=1\r\n", (uint8_t*)"OK");  //WIFI模块设置为STA模式
        SendAT((uint8_t*)"AT+CWAUTOCONN=1\r\n", (uint8_t*)"OK");  //上电自动连接到AP
        SendAT((uint8_t*)"AT+CWSTARTSMART=2\r\n", (uint8_t*)"OK"); //开启SmartConfig(SmartConfig类型为AirKiss)

        printf("Start SmartConfig:\r\n");
        //等待配网，3分钟超时退出
        Timer6.usDelay_Timer = 0;
        while(Timer6.usDelay_Timer < TIMER6_3min)
        {
            //DMA重新接收设置
            ESP8266.DMA_Receive_Set();

            //LED2指示灯快闪
            HAL_Delay(100);
            LED_WIFI_TOGGLE;

            //打印信息
            printf("%s", UART3.pucRec_Buffer);
            printf("%s", UART3.pucRec_Buffer);

            //判断获取AP
            if(strstr((const char*)UART3.pucRec_Buffer, "Smart get wifi info") != NULL) //获取到AP信息
            {
                Timer6.usDelay_Timer = TIMER6_3min - TIMER6_10S;
            }

            //判断连接AP
            if(strstr((const char*)UART3.pucRec_Buffer, "connected wifi") != NULL) //成功连接到AP
            {
                SendAT((uint8_t*)"AT+CWSTOPSMART\r\n", (uint8_t*)"OK"); //停止SmartConfig
                break;
            }
        }

        //输出信息
        if(Timer6.usDelay_Timer < TIMER6_3min)
        {
            printf("\r\n\r\nSmartconfig Success!\r\n");
            ESP8266.TCP_Connect_Status  = FALSE;
            ESP8266.TCP_Reconnect_Timer = TIMER6_10S; //立马连接TCP
            //清除配网标志位
            ESP8266.SmartConifg_Flag = FALSE;
        }
        else
        {
            printf("\r\n\r\nSmartconfig Fail!\r\n");
            ESP8266.TCP_Connect_Status  = FALSE;
            ESP8266.TCP_Reconnect_Timer = 0;
        }
        //关闭指示灯
        LED_WIFI_OFF;
    }
}

/*
	* @name   TCP_Connect_Server
	* @brief  通过TCP连接服务器
	* @param  None
	* @retval None
*/
static void TCP_Connect_Server()
{
    uint8_t AP_Connect_Flag = FALSE; //AP连接标志位，为TRUE，才进行TCP连接

    SendAT((uint8_t*)"AT\r\n", (uint8_t*)"OK");             //测试AT
    SendAT((uint8_t*)"ATE0\r\n", (uint8_t*)"OK");           //关闭回显
    SendAT((uint8_t*)"AT+CWMODE_CUR=1\r\n", (uint8_t*)"OK"); //WIFI模块设置为STA模式

    //获取IP
    Timer6.usDelay_Timer = 0;
    do
    {
        //DMA重新接收设置
        ESP8266.DMA_Receive_Set();
        //发送AT指令
        UART3.SendString((uint8_t*)"AT+CIFSR\r\n");
        //延时1000ms，等待接收完成
        HAL_Delay(1000);

        //打印信息
        printf("%s", UART3.pucRec_Buffer);

        //10s没有获取IP，跳出等待
        if(Timer6.usDelay_Timer >= TIMER6_10S)
        {
            ESP8266.Error();
            break;
        }
    }
    while(strstr((const char*)UART3.pucRec_Buffer, "0.0.0.0") != NULL);
    //控制WIFI指示灯
    if(Timer6.usDelay_Timer < TIMER6_10S)
    {
        LED_WIFI_ON;
        AP_Connect_Flag = TRUE;
        printf("AP connect success!\r\n");
    }
    else
    {
        LED_WIFI_OFF;
        AP_Connect_Flag = FALSE;
        printf("AP connect fail,auto entry Smartconfig mode!\r\n");
        ESP8266.SmartConifg_Flag = TRUE;
    }

    //等待稳定连接到AP
    Timer6.usDelay_Timer = 0;
    do
    {
        //DMA重新接收设置
        ESP8266.DMA_Receive_Set();
        //发送AT指令
        UART3.SendString((uint8_t*)"AT\r\n");
        //延时100ms，等待接收完成
        HAL_Delay(100);

        //打印信息
        printf("%s", UART3.pucRec_Buffer);

        //10s没有同步，超时错误处理
        if(Timer6.usDelay_Timer >= TIMER6_10S)
        {
            ESP8266.Error();
            break;
        }
    }
    while(strstr((const char*)UART3.pucRec_Buffer, "OK") == NULL);

    //连接服务器
    if(AP_Connect_Flag == TRUE)
    {
        printf("Connect TCP Server!\r\n");

        Timer6.usDelay_Timer = 0;
        do
        {
            //DMA重新接收设置
            ESP8266.DMA_Receive_Set();
            //发送AT指令
            UART3.SendString(TCP_Server);
            //延时500ms，等待接收完成
            HAL_Delay(500);

            //打印信息
            printf("%s", UART3.pucRec_Buffer);

            //10s没有连接到TCP Server
            if(Timer6.usDelay_Timer >= TIMER6_10S)
            {
                printf("Connect TCP Server Failure!\r\n");

                ESP8266.TCP_Connect_Status = FALSE;
                LED_WIFI_OFF;
                break;
            }

            //连接到服务器
            if(strstr((const char*)UART3.pucRec_Buffer, "CONNECT") != NULL)
            {
                printf("Connect TCP Server Success!\r\n");

                //使能透传模式
                SendAT((uint8_t*)"AT+CIPMODE=1\r\n", (uint8_t*)"OK"); //使能透传
                SendAT((uint8_t*)"AT+CIPSEND\r\n", (uint8_t*)">");   //开始发送

                ESP8266.TCP_Connect_Status = TRUE;
                LED_WIFI_ON;
                break;
            }
        }
        while(strstr((const char*)UART3.pucRec_Buffer, "CONNECT") == NULL);
        UART3.SendString(SERIAL_NUM);
        HAL_Delay(500);
        
        //DMA重新接收设置，开始接收TCP服务器的指令
        ESP8266.DMA_Receive_Set();
    }
}

/*
	* @name   Transfer_SHT30
	* @brief  传送SHT30的温湿度
	* @param  NONE
	* @retval None
*/
static void Transfer_SHT30()
{
    //数据传送
//    UART3.SendString((uint8_t*)"The Humidity of SHT30 is ");
//    HAL_Delay(3);  //延时，等待字符串发送完   时间 = (11*1000ms)/115200 * 25 = 2.39ms
//    UART3.SendArray(UART3.pucSend_Buffer, 5);
    // #27.6,49,55,#
    sprintf((char*)UART3.pucSend_Buffer, "#%.1f℃,%d%%RH,%d%%,%d%%,%d,%d,%d,#", SHT30.fTemperature, SHT30.ucHumidity, \
        tBrightness.eResult, tMQ_2.AQI, tFAN.eFlag, tPowerLED.Status, tRelay.eFlag);
    UART3.SendString(UART3.pucSend_Buffer);
    HAL_Delay(10); // 时间 = (11*1000ms)/115200 * 25 = 2.39ms
    memset(UART3.pucSend_Buffer, 0, UART3_Send_LENGTH);
}

/*
	* @name   Receive_Information
	* @brief  接收信息
	* @param  None
	* @retval None
*/
static void Receive_Information()
{
    if(ESP8266.TCP_Connect_Status == TRUE)
    {
        printf("Rec info from TCP ser:\r\n");
        printf("%s", UART3.pucRec_Buffer);
        printf("\r\n");

        //切换继电器状态
        if(strstr((const char*)UART3.pucRec_Buffer, "FAN_1") != NULL)
        {
            tFAN.ON();
        }

        if(strstr((const char*)UART3.pucRec_Buffer, "FAN_0") != NULL)
        {
            tFAN.OFF();
        }

        if(strstr((const char*)UART3.pucRec_Buffer, "RELAY_1") != NULL)
        {
            tRelay.ON();
        }

        if(strstr((const char*)UART3.pucRec_Buffer, "RELAY_0") != NULL)
        {
            tRelay.OFF();
        }

        if(strstr((const char*)UART3.pucRec_Buffer, "LED_1") != NULL)
        {
            tPowerLED.ON();
        }

        if(strstr((const char*)UART3.pucRec_Buffer, "LED_0") != NULL)
        {
            tPowerLED.OFF();
        }

        //DMA重新接收设置，接收TCP服务器的新指令
        ESP8266.DMA_Receive_Set();
    }

}

/*
	* @name   DMA_Receive_Set
	* @brief  DMA接收设置
	* @param  None
	* @retval None
*/
static void DMA_Receive_Set(void)
{
    //串口2禁止DMA接收
    HAL_UART_DMAStop(&huart3);
    //清缓存
//    memset(UART3.pucRec_Buffer, 0, strlen((const char*)UART3.pucRec_Buffer));
    memset(UART3.pucRec_Buffer, 0, UART3_Rec_LENGTH);
    //串口3开启DMA接收
    HAL_UART_Receive_DMA(&huart3, UART3.pucRec_Buffer, UART3_Rec_LENGTH);
}

/*
	* @name   Error
	* @brief  错误信息
	* @param  None
	* @retval None
*/
static void Error()
{
    HAL_GPIO_WritePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin, GPIO_PIN_RESET);
}
/*************************************
  End Of File
*************************************/
