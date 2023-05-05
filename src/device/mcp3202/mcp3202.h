#ifndef __MCP3202_H__
#define __MCP3202_H__

#include "my_lpc1114fn28.h"

extern uint32_t MCP3202_Read_Reg(uint32_t addr);
extern void MCP3202_Config_Request(void);

#endif // __MCP3202_H__
