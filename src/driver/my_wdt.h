#ifndef __MY_WDT_H__
#define __MY_WDT_H__

#include "chip.h"

/* Comment this define to let the watchdog timeout. In this case, the board
   will continuously drop via to the WDT warning. */
#define DISABLE_WDT_TIMEOUT


//============
// Prototypes
//============
void WDT_Config_Request(void);

#endif // __MY_ADC_H__

//=========================================================
// End of Program
//=========================================================
