/* Includes ------------------------------------------------------------------*/
#include "ESP8266.h"

/* Private define-------------------------------------------------------------*/
//#define TCP_Server  (uint8_t*)"AT+CIPSTART=\"TCP\",\"192.168.31.19\",8888\r\n"  //���Զ�ģ���TCP������
#define TCP_Server  (uint8_t*)"AT+CIPSTART=\"TCP\",\"112.74.142.132\",8647\r\n"  //���Զ�ģ���TCP������
#define SERIAL_NUM (uint8_t *)"97783ANHE3SEK8U5\r\n"
//#define TCP_Server  (uint8_t*)"AT+CIPSTART=\"TCP\",\"192.168.1.132\",8888\r\n"  //�ֻ���ģ���TCP������

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/
static void Init(void);                            //ESP8266��ʼ��
static void SmartConifg(void);                     //WIFIģ������
static void TCP_Connect_Server(void);              //ͨ��TCP���ӷ�����
static void Transfer_SHT30(void);                  //����SHT30����ʪ��
static void Receive_Information(void);             //������Ϣ
static void DMA_Receive_Set(void);                 //DMA��������
static void Error(void);                           //������Ϣ

static void SendAT(uint8_t*, uint8_t*);            //����ATָ��

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
	* @brief  ESP8266��ʼ��
	* @param  None
	* @retval None
*/
static void Init()
{
    //��λģ��
    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);

    //ʹ��ģ��
    HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET);
    //��ʱ500ms���ȴ�WIFIģ���ȶ���׼���ý���ATָ��
    HAL_Delay(500);

    //ͬ��������
    Timer6.usDelay_Timer = 0;
    do
    {
        //DMA���½�������
        ESP8266.DMA_Receive_Set();

        //����ATָ��
        UART3.SendString((uint8_t*)"AT\r\n");
        //��ʱ100ms���ȴ��������
        HAL_Delay(100);

        //��ӡ��Ϣ
        printf("%s", UART3.pucRec_Buffer);

        //��ʱ����
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
	* @brief  ����ATָ��
	* @param  AT_Command  -> �����͵�ATָ��
	          Respond_Str -> ��Ӧ�����а������ַ���
	* @retval None
*/
static void SendAT(uint8_t* AT_Command, uint8_t* Respond_Str)
{
    uint8_t* const  Ptr_AT_Command   = AT_Command;
    uint8_t* const  Ptr_Respond_Str  = Respond_Str;

    //DMA���½�������
    ESP8266.DMA_Receive_Set();

    //����ATָ��
    UART3.SendString(Ptr_AT_Command);
    //��ӡ��Ϣ
    printf("%s", Ptr_AT_Command);

    //�ȴ�ģ���Ӧ���ݣ���ʱ������
    Timer6.usDelay_Timer = 0;
    while(strstr((const char*)UART3.pucRec_Buffer, "\r\n") == NULL)
    {
        if(Timer6.usDelay_Timer > TIMER6_100mS)
        {
            ESP8266.Error();
            break;
        }
    }
    //��ʱ10ms��������ȫ���ֽ�
    HAL_Delay(10);

    //ģ���Ӧ���ݴ�����
    if(strstr((const char*)UART3.pucRec_Buffer, (const char*)Ptr_Respond_Str) == NULL)
    {
        ESP8266.Error();
    }

    //��ӡ��Ϣ
    printf("%s", UART3.pucRec_Buffer);
}

/*
	* @name   SmartConifg
	* @brief  WIFIģ������
	* @param  None
	* @retval None
*/
static void SmartConifg()
{
    if(ESP8266.SmartConifg_Flag == TRUE)
    {
        //�ر�ָʾ��

        //�˳�͸��ģʽ
        *(UART3.pucSend_Buffer + 0) = '+';
        *(UART3.pucSend_Buffer + 1) = '+';
        *(UART3.pucSend_Buffer + 2) = '+';
        UART3.SendArray(UART3.pucSend_Buffer, 3);
        HAL_Delay(1000);

        //����
        SendAT((uint8_t*)"AT\r\n", (uint8_t*)"OK");               //����AT
        SendAT((uint8_t*)"ATE0\r\n", (uint8_t*)"OK");             //�رջ���
        SendAT((uint8_t*)"AT+CWMODE_CUR=1\r\n", (uint8_t*)"OK");  //WIFIģ������ΪSTAģʽ
        SendAT((uint8_t*)"AT+CWAUTOCONN=1\r\n", (uint8_t*)"OK");  //�ϵ��Զ����ӵ�AP
        SendAT((uint8_t*)"AT+CWSTARTSMART=2\r\n", (uint8_t*)"OK"); //����SmartConfig(SmartConfig����ΪAirKiss)

        printf("Start SmartConfig:\r\n");
        //�ȴ�������3���ӳ�ʱ�˳�
        Timer6.usDelay_Timer = 0;
        while(Timer6.usDelay_Timer < TIMER6_3min)
        {
            //DMA���½�������
            ESP8266.DMA_Receive_Set();

            //LED2ָʾ�ƿ���
            HAL_Delay(100);
            LED_WIFI_TOGGLE;

            //��ӡ��Ϣ
            printf("%s", UART3.pucRec_Buffer);
            printf("%s", UART3.pucRec_Buffer);

            //�жϻ�ȡAP
            if(strstr((const char*)UART3.pucRec_Buffer, "Smart get wifi info") != NULL) //��ȡ��AP��Ϣ
            {
                Timer6.usDelay_Timer = TIMER6_3min - TIMER6_10S;
            }

            //�ж�����AP
            if(strstr((const char*)UART3.pucRec_Buffer, "connected wifi") != NULL) //�ɹ����ӵ�AP
            {
                SendAT((uint8_t*)"AT+CWSTOPSMART\r\n", (uint8_t*)"OK"); //ֹͣSmartConfig
                break;
            }
        }

        //�����Ϣ
        if(Timer6.usDelay_Timer < TIMER6_3min)
        {
            printf("\r\n\r\nSmartconfig Success!\r\n");
            ESP8266.TCP_Connect_Status  = FALSE;
            ESP8266.TCP_Reconnect_Timer = TIMER6_10S; //��������TCP
            //���������־λ
            ESP8266.SmartConifg_Flag = FALSE;
        }
        else
        {
            printf("\r\n\r\nSmartconfig Fail!\r\n");
            ESP8266.TCP_Connect_Status  = FALSE;
            ESP8266.TCP_Reconnect_Timer = 0;
        }
        //�ر�ָʾ��
        LED_WIFI_OFF;
    }
}

/*
	* @name   TCP_Connect_Server
	* @brief  ͨ��TCP���ӷ�����
	* @param  None
	* @retval None
*/
static void TCP_Connect_Server()
{
    uint8_t AP_Connect_Flag = FALSE; //AP���ӱ�־λ��ΪTRUE���Ž���TCP����

    SendAT((uint8_t*)"AT\r\n", (uint8_t*)"OK");             //����AT
    SendAT((uint8_t*)"ATE0\r\n", (uint8_t*)"OK");           //�رջ���
    SendAT((uint8_t*)"AT+CWMODE_CUR=1\r\n", (uint8_t*)"OK"); //WIFIģ������ΪSTAģʽ

    //��ȡIP
    Timer6.usDelay_Timer = 0;
    do
    {
        //DMA���½�������
        ESP8266.DMA_Receive_Set();
        //����ATָ��
        UART3.SendString((uint8_t*)"AT+CIFSR\r\n");
        //��ʱ1000ms���ȴ��������
        HAL_Delay(1000);

        //��ӡ��Ϣ
        printf("%s", UART3.pucRec_Buffer);

        //10sû�л�ȡIP�������ȴ�
        if(Timer6.usDelay_Timer >= TIMER6_10S)
        {
            ESP8266.Error();
            break;
        }
    }
    while(strstr((const char*)UART3.pucRec_Buffer, "0.0.0.0") != NULL);
    //����WIFIָʾ��
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

    //�ȴ��ȶ����ӵ�AP
    Timer6.usDelay_Timer = 0;
    do
    {
        //DMA���½�������
        ESP8266.DMA_Receive_Set();
        //����ATָ��
        UART3.SendString((uint8_t*)"AT\r\n");
        //��ʱ100ms���ȴ��������
        HAL_Delay(100);

        //��ӡ��Ϣ
        printf("%s", UART3.pucRec_Buffer);

        //10sû��ͬ������ʱ������
        if(Timer6.usDelay_Timer >= TIMER6_10S)
        {
            ESP8266.Error();
            break;
        }
    }
    while(strstr((const char*)UART3.pucRec_Buffer, "OK") == NULL);

    //���ӷ�����
    if(AP_Connect_Flag == TRUE)
    {
        printf("Connect TCP Server!\r\n");

        Timer6.usDelay_Timer = 0;
        do
        {
            //DMA���½�������
            ESP8266.DMA_Receive_Set();
            //����ATָ��
            UART3.SendString(TCP_Server);
            //��ʱ500ms���ȴ��������
            HAL_Delay(500);

            //��ӡ��Ϣ
            printf("%s", UART3.pucRec_Buffer);

            //10sû�����ӵ�TCP Server
            if(Timer6.usDelay_Timer >= TIMER6_10S)
            {
                printf("Connect TCP Server Failure!\r\n");

                ESP8266.TCP_Connect_Status = FALSE;
                LED_WIFI_OFF;
                break;
            }

            //���ӵ�������
            if(strstr((const char*)UART3.pucRec_Buffer, "CONNECT") != NULL)
            {
                printf("Connect TCP Server Success!\r\n");

                //ʹ��͸��ģʽ
                SendAT((uint8_t*)"AT+CIPMODE=1\r\n", (uint8_t*)"OK"); //ʹ��͸��
                SendAT((uint8_t*)"AT+CIPSEND\r\n", (uint8_t*)">");   //��ʼ����

                ESP8266.TCP_Connect_Status = TRUE;
                LED_WIFI_ON;
                break;
            }
        }
        while(strstr((const char*)UART3.pucRec_Buffer, "CONNECT") == NULL);
        UART3.SendString(SERIAL_NUM);
        HAL_Delay(500);
        
        //DMA���½������ã���ʼ����TCP��������ָ��
        ESP8266.DMA_Receive_Set();
    }
}

/*
	* @name   Transfer_SHT30
	* @brief  ����SHT30����ʪ��
	* @param  NONE
	* @retval None
*/
static void Transfer_SHT30()
{
    //���ݴ���
//    UART3.SendString((uint8_t*)"The Humidity of SHT30 is ");
//    HAL_Delay(3);  //��ʱ���ȴ��ַ���������   ʱ�� = (11*1000ms)/115200 * 25 = 2.39ms
//    UART3.SendArray(UART3.pucSend_Buffer, 5);
    // #27.6,49,55,#
    sprintf((char*)UART3.pucSend_Buffer, "#%.1f��,%d%%RH,%d%%,%d%%,%d,%d,%d,#", SHT30.fTemperature, SHT30.ucHumidity, \
        tBrightness.eResult, tMQ_2.AQI, tFAN.eFlag, tPowerLED.Status, tRelay.eFlag);
    UART3.SendString(UART3.pucSend_Buffer);
    HAL_Delay(10); // ʱ�� = (11*1000ms)/115200 * 25 = 2.39ms
    memset(UART3.pucSend_Buffer, 0, UART3_Send_LENGTH);
}

/*
	* @name   Receive_Information
	* @brief  ������Ϣ
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

        //�л��̵���״̬
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

        //DMA���½������ã�����TCP����������ָ��
        ESP8266.DMA_Receive_Set();
    }

}

/*
	* @name   DMA_Receive_Set
	* @brief  DMA��������
	* @param  None
	* @retval None
*/
static void DMA_Receive_Set(void)
{
    //����2��ֹDMA����
    HAL_UART_DMAStop(&huart3);
    //�建��
//    memset(UART3.pucRec_Buffer, 0, strlen((const char*)UART3.pucRec_Buffer));
    memset(UART3.pucRec_Buffer, 0, UART3_Rec_LENGTH);
    //����3����DMA����
    HAL_UART_Receive_DMA(&huart3, UART3.pucRec_Buffer, UART3_Rec_LENGTH);
}

/*
	* @name   Error
	* @brief  ������Ϣ
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
