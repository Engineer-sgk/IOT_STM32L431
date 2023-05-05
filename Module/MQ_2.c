#include "MQ_2.h"

extern uint32_t ADC_Value[2];

static void MQ_2_Detect(void);

MQ_2_T tMQ_2 = 
{
    0,
    MQ_2_Detect,
};

void MQ_2_Detect(void)
{
    uint16_t uiADC_Temp;
   
    uiADC_Temp = ADC_Value[1];
    tMQ_2.AQI = 100 - (100 * ((float)uiADC_Temp / 4095));
    printf("AQI = %d.\r\n", tMQ_2.AQI);
}


