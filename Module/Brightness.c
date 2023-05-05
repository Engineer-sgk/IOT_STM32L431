#include "Brightness.h"

extern uint32_t ADC_Value[2];

static void Brightness_Detect(void);

Brightness_T tBrightness = 
{
    0,
    Brightness_Detect,
};

static void Brightness_Detect(void)
{
    uint16_t uiADC_TEMP = 0;
    
    uiADC_TEMP = ADC_Value[0];
    tBrightness.eResult = 100 - (100 * ((float)uiADC_TEMP / 4095u));
    printf("Bright = %d.\r\n", tBrightness.eResult);

}
