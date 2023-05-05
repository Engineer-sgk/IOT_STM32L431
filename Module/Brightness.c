#include "Brightness.h"

extern uint32_t ADC_Value[2];

#define BRIGHT_PRIORITY 11u
#define BRIGHT_TIMESLICE 15u

//ALIGN(RT_ALIGN_SIZE)
static char BRIGHT_STACK[1024];
static struct rt_thread Bright_Thread;

static void Brightness_Detect(void);
static void Bright_Init(void);
static void Bright_Thread_Entry(void * paramter);

Brightness_T tBrightness = 
{
    0,
    Bright_Init,
    Brightness_Detect,
};

static void Bright_Init(void)
{
    rt_err_t err;
    
    err = rt_thread_init(&Bright_Thread,
                         "Bright",
                          Bright_Thread_Entry,
                          RT_NULL,
                          &BRIGHT_STACK[0],
                          sizeof(BRIGHT_STACK),
                          BRIGHT_PRIORITY,
                          BRIGHT_TIMESLICE);
                          
    if(err != RT_ERROR)
    {
        err = rt_thread_startup(&Bright_Thread);
    }
    
    if(err == RT_ERROR)
    {
        rt_kprintf("BRIGHT Init fail.\r\n");
    }
}

static void Bright_Thread_Entry(void * paramter)
{
    while(1)
    {
        Brightness_Detect();
        rt_kprintf("Bright = %d.\r\n", tBrightness.eResult);
        rt_thread_mdelay(1000);
    }
}

static void Brightness_Detect(void)
{
    uint16_t uiADC_TEMP = 0;
    
    uiADC_TEMP = ADC_Value[0];
    tBrightness.eResult = 100 - (100 * ((float)uiADC_TEMP / 4095u));
//    printf("Bright = %d.\r\n", tBrightness.eResult);

}
