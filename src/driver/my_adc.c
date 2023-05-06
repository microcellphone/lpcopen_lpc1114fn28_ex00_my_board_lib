#include "my_adc.h"

#define ADC_CH      ADC_CH0
#define ADC_CLKDIV  7

static ADC_CLOCK_SETUP_T ADCSetup;

//void ADC_Config_Request(ADC_CHANNEL_T channel)
void ADC_Config_Request(void)
{
	Chip_ADC_Init(LPC_ADC, &ADCSetup);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);
#if ADC_INTERRUPT_MODE == 1
	Chip_ADC_SetSampleRate(LPC_ADC, NULL, ADC_CLKDIV);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH, ENABLE);
	Chip_ADC_Int_SetGlobalCmd(LPC_ADC, ENABLE);
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
#endif
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
