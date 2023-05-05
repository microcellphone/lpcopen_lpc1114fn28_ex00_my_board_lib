#ifndef __PWM_H__
#define __PWM_H__
#include "my_lpc1114fn28.h"

#define MIN_PWM 		  0
#define MAX_PWM 		999

#define PWM_FREQ_HZ 20000

typedef enum pwm_mr {
	PWM_MR0 = 0,
	PWM_MR1 = 1,
	PWM_MR2 = 2,
	PWM_MR3 = 3,
} pwm_mr_t;

extern uint32_t DutyCycle0;
extern uint32_t DutyCycle1;

//============
// Prototypes
//============
extern void PWM_Config_Request(LPC_TIMER_T *pTMR);
void PWM_Set_DutyCycle(uint32_t channel, uint32_t duty_cycle);

#endif // __PWM_H__
