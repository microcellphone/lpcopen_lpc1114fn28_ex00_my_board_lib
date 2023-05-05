#ifndef __MY_TIMER_H__
#define __MY_TIMER_H__
#include "chip.h"

//============
// Prototypes
//============
extern void TIMER_Config_Request(LPC_TIMER_T *pTMR, uint32_t tickrate_hz);
extern void TIMER_Capture_Request(LPC_TIMER_T *pTMR, uint32_t tickrate_hz);
//

#endif // __TIMER16_H__

//=========================================================
// End of Program
//=========================================================
