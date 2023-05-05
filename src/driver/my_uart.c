#include "chip.h"
#include "my_uart.h"

volatile uint32_t UARTStatus;

#define BUFF_SIZE	64	/* Length of Receive/Transmission FIFO */

static volatile struct {
  uint16_t	ri, wi, ct, act;
  uint8_t		buff[BUFF_SIZE];
} TxBuff, RxBuff;

/*****************************************************************************
** Function name:		UART_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
//void UART_IRQHandler(void)
void UART_IRQHandler_ISR(void)
{
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;
	unsigned char Buf;
	uint8_t d;
	int i, cnt;


	IIRValue = Chip_UART_ReadIntIDReg(LPC_USART);
	IIRValue >>= 1;			/* skip pending bit in IIR */
	IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */

	if (IIRValue == IIR_RLS){		/* Receive Line Status */
		LSRValue = Chip_UART_ReadLineStatus(LPC_USART);
		/* Receive Line Status */
		if (LSRValue & (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_RXFE | UART_LSR_BI)){
			/* There are errors or break interrupt */
			/* Read LSR will clear the interrupt */
			UARTStatus = LSRValue;
			Dummy = Chip_UART_ReadByte(LPC_USART);	/* Dummy read on RX to clear interrupt, then bail out */
			return;
		}
		if (LSRValue & UART_LSR_RDR){	/* Receive Data Ready */
			/* If no error on RLS, normal ready, save into the data buffer. */
			/* Note: read RBR will clear the interrupt */
			Buf = Chip_UART_ReadByte(LPC_USART);
		}
	}else if (IIRValue == IIR_RDA){	/* Receive Data Available */
		/* Receive Data Available */
		i = RxBuff.wi;
		cnt = RxBuff.ct;
		while (Chip_UART_ReadLineStatus(LPC_USART) & UART_LSR_RDR) {	/* Get all data in the Rx FIFO */
			d = Chip_UART_ReadByte(LPC_USART);
			if (cnt < BUFF_SIZE) {	/* Store data if Rx buffer is not full */
				RxBuff.buff[i++] = d;
				i %= BUFF_SIZE;
				cnt++;
			}
		}
		RxBuff.wi = i;
		RxBuff.ct = cnt;
	}else if (IIRValue == IIR_CTI){	/* Character timeout indicator */
		/* Character Time-out indicator */
		UARTStatus |= 0x100;		/* Bit 9 as the CTI error */
	}else if (IIRValue == IIR_THRE){	/* THRE, transmit holding register empty */
		cnt = TxBuff.ct;
		if(cnt){/* There is one or more byte to send */
			i = TxBuff.ri;
			for (d = 16; d && cnt; d--, cnt--){	/* Fill Tx FIFO */
				Chip_UART_SendByte(LPC_USART, TxBuff.buff[i++]);
				i %= BUFF_SIZE;
			}
			TxBuff.ri = i;
			TxBuff.ct = cnt;
		}else{
			TxBuff.act = 0; /* When no data to send, next putc() must trigger Tx sequense */
		}
	}
	return;
}

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
