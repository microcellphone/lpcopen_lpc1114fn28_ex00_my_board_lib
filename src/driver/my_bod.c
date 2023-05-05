#include "my_bod.h"


void BOD_Config_Request(void)
{
	uint32_t sysResetStatus;

	/* Set brown-out interrupt level with reset  */
	Chip_SYSCTL_SetBODLevels(SYSCTL_BODRSTLVL_2_06V, SYSCTL_BODINTVAL_2_80V);
	Chip_SYSCTL_EnableBODReset();

	/* Enable BOD interrupt */
	NVIC_ClearPendingIRQ(BOD_IRQn);
	NVIC_EnableIRQ(BOD_IRQn);

	/* If the board was reset due to a BOD event, the reset can be
	   detected here. If the board was completely powered off, the BOD
	   reset event won't be active. */
	sysResetStatus = Chip_SYSCTL_GetSystemRSTStatus();
	if ((sysResetStatus & SYSCTL_RST_BOD) == 0) {
		/* Board was reset via a normal power-on reset event */
		sysResetStatus |= SYSCTL_RST_POR;
	} else {
		/* Board was reset via a BOD event */
		sysResetStatus |= SYSCTL_RST_BOD;
	}

	/* Clear reset status */
	Chip_SYSCTL_ClearSystemRSTStatus(sysResetStatus);
}

