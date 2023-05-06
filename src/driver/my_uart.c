#include "chip.h"
#include "my_uart.h"

uint32_t UARTStatus;

UART_BUFF_T TxBuff;
UART_BUFF_T RxBuff;

/*****************************************************************************
** Function name:		UART_Config_Request
**
** Descriptions:		Initialize UART0 port, setup pin select,
**				clock, parity, stop bits, FIFO, etc.
**
** parameters:			UART baudrate
** Returned value:		None
** 
*****************************************************************************/
void UART_Config_Request(uint32_t baudrate)
{
	/* Clear Tx/Rx buffers */
	TxBuff.ri = 0; TxBuff.wi = 0; TxBuff.ct = 0; TxBuff.act = 0;
	RxBuff.ri = 0; RxBuff.wi = 0; RxBuff.ct = 0;

	NVIC_DisableIRQ(UART0_IRQn);

	/* Setup UART for baudrate 8N1 */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, baudrate);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS));
	Chip_UART_TXEnable(LPC_USART);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_THREINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);

  return;
}

///////////////////////////////////////////////////////////////////////////////

void uart0_putc(uint8_t d)
{
  int32_t i;

  /* Wait for Tx buffer ready */
  while (TxBuff.ct >= BUFF_SIZE) ;

  __disable_irq();
  if(TxBuff.act){
  	i = TxBuff.wi;		/* Put a byte into Tx byffer */
  	TxBuff.buff[i++] = d;
  	TxBuff.wi = i % BUFF_SIZE;
  	TxBuff.ct++;
  }else{
  	TxBuff.act = 1;		/* Trigger Tx sequense */
	Chip_UART_SendByte(LPC_USART, d);
  }
  __enable_irq();
}

uint8_t uart0_getc(void)
{
	uint8_t d;
	int i;

	/* Wait while Rx buffer is empty */
	while (!RxBuff.ct) ;

	i = RxBuff.ri;			/* Get a byte from Rx buffer */
	d = RxBuff.buff[i++];
	RxBuff.ri = i % BUFF_SIZE;
	__disable_irq();
	RxBuff.ct--;
	__enable_irq();

	return d;
}
