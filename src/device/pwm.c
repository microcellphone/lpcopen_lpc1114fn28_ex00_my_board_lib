#include "pwm.h"

uint32_t DutyCycle0;
uint32_t DutyCycle1;

void PWM_Config_Request(LPC_TIMER_T *pTMR)
{
    Chip_TIMER_Init(pTMR);

    Chip_TIMER_PrescaleSet(pTMR, 0);
    Chip_TIMER_SetMatch(pTMR, PWM_MR2, MAX_PWM);
    Chip_TIMER_MatchEnableInt(pTMR, PWM_MR2);
    Chip_TIMER_ResetOnMatchEnable(pTMR, PWM_MR2);

    pTMR->PWMC = 0b1011; // PWMs are on MAT3, MAT1 and MAT0
    Chip_TIMER_SetMatch(pTMR, PWM_MR0, MIN_PWM);
    Chip_TIMER_SetMatch(pTMR, PWM_MR1, MIN_PWM);
    Chip_TIMER_SetMatch(pTMR, PWM_MR3, MIN_PWM);
    Chip_TIMER_Enable(pTMR);

    if(pTMR == LPC_TIMER16_0){
    	NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);
    	NVIC_EnableIRQ(TIMER_16_0_IRQn);
    }
    if(pTMR == LPC_TIMER16_1){
    	NVIC_ClearPendingIRQ(TIMER_16_1_IRQn);
    	NVIC_EnableIRQ(TIMER_16_1_IRQn);
    }
    if(pTMR == LPC_TIMER32_0){
    	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
    	NVIC_EnableIRQ(TIMER_32_0_IRQn);
    }
    if(pTMR == LPC_TIMER32_1){
    	NVIC_ClearPendingIRQ(TIMER_32_1_IRQn);
    	NVIC_EnableIRQ(TIMER_32_1_IRQn);
    }
}

void PWM_Set_DutyCycle(uint32_t channel, uint32_t duty_cycle)
{
	uint32_t dc;

	dc = (duty_cycle > MAX_PWM)? MAX_PWM : duty_cycle;
    if (channel == 0) DutyCycle0 = dc;
    if (channel == 1) DutyCycle1 = dc;
}
