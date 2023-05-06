#include "chip.h"
#include "mcp3202.h"

#include <my_ssp.h>

extern void CS_Init(void);

#define CS_PORT_NUM		0
#define CS_PIN_NUM		2

uint8_t sspreadbuf[2];

void CS_Init(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, (IOCON_FUNC0 | IOCON_MODE_INACT));
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 2);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, true);
}


void CS_Force(int32_t state)
{
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, state);
}


void MCP3202_Config_Request(void)
{
	SPI_Config_Request(8, SSP_SLOW, CS_AUTO);
}


uint32_t MCP3202_Read_Reg(uint32_t addr)
{
  uint8_t tx_data[2];
  uint8_t chanBits;

  if (addr == 0) {
  	tx_data [0] = 0b11010000 ;
  } else {
  	tx_data [0] = 0b11110000 ;
  }
  tx_data [1] = 0;

  CS_Force(0);
  SPI_Tx_Rx_Data(tx_data, 2, sspreadbuf, 2);
  CS_Force(1);

  return ((sspreadbuf [0] << 7) | (sspreadbuf [1] >> 1)) & 0xFFF ;
}
