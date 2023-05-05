#include "my_adc.h"

static ADC_CLOCK_SETUP_T ADCSetup;

//void ADC_Config_Request(ADC_CHANNEL_T channel)
void ADC_Config_Request(void)
{
	Chip_ADC_Init(LPC_ADC, &ADCSetup);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
}

//uint32_t Get_ADC_Data(ADC_CHANNEL_T channel)
uint32_t ADC_Get_Data(uint32_t channel)
{
    uint16_t dataADC;

	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH0, ADC_DR_DONE_STAT) != SET) {}
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &dataADC);

    return dataADC;

}
