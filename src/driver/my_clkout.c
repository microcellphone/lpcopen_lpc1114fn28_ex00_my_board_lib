#include "my_clkout.h"

void CLKOUT_Config_Request(void)
{
}

void CLKOUT_Set_Request(uint32_t clock)
{
	if(clock >= SYSCTL_CLKOUTSRC_IRC && clock<= SYSCTL_CLKOUTSRC_MAINSYSCLK)
		Chip_Clock_SetCLKOUTSource(clock, 1);
	else
		while(1);
}

void CLKOUT_Stop_Request(void)
{
		Chip_Clock_SetCLKOUTSource(SYSCTL_CLKOUTSRC_IRC, 0);
}
