#ifndef __MY_ADC_H__
#define __MY_ADC_H__

#include "chip.h"

void ADC_Config_Request(void);
uint32_t ADC_Get_Data(uint32_t channel);

#endif // __MY_ADC_H__
