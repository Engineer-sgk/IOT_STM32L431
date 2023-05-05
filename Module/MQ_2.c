#include "MQ_2.h"

extern uint32_t ADC_Value[2];

#define MQ_2_PRIORITY 12u
#define MQ_2_TIMESLICE 15u

//ALIGN(RT_ALIGN_SIZE)
static char MQ_2_Stack[1024];
static struct rt_thread MQ_2_Thread;

static void MQ_2_Detect(void);
static void MQ_2_Init(void);
static void MQ_2_Thread_Entry(void * paramter);

MQ_2_T tMQ_2 = 
{
    0,
    MQ_2_Init,
    MQ_2_Detect,
};

static void MQ_2_Init(void)
{
    rt_err_t err;
    
    err = rt_thread_init(&MQ_2_Thread,
                         "MQ_2",
                         MQ_2_Thread_Entry,
                         RT_NULL,
                         &MQ_2_Stack[0],
                         sizeof(MQ_2_Stack),
                         MQ_2_PRIORITY,
                         MQ_2_TIMESLICE);
                         
    if(err != RT_ERROR)
    {
        err = rt_thread_startup(&MQ_2_Thread);
    }        
    
    if(err == RT_ERROR)
    {
        rt_kprintf("MQ_2 Init fail.\r\n");
    }
}

static void MQ_2_Thread_Entry(void *paramter)
{
    while(1)
    {
        MQ_2_Detect();
        rt_kprintf("AQI = %d.\r\n", tMQ_2.AQI);
        rt_thread_mdelay(1000u);
    }
}

static void MQ_2_Detect(void)
{
    uint16_t uiADC_Temp;
   
    uiADC_Temp = ADC_Value[1];
    tMQ_2.AQI = 100 - (100 * ((float)uiADC_Temp / 4095));
//    printf("AQI = %d.\r\n", tMQ_2.AQI);
}


