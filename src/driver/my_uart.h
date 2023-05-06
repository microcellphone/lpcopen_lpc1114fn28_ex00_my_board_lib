/*****************************************************************************
 *   uart.h:  Header file for NXP LPC1xxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.12.07  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __MY_UART_H
#define __MY_UART_H

#include<stdint.h>
#include "chip.h"

#define BUFF_SIZE	64	/* Length of Receive/Transmission FIFO */

typedef struct {
  uint16_t	ri, wi, ct, act;
  uint8_t		buff[BUFF_SIZE];
} UART_BUFF_T;

extern uint32_t UARTStatus;
extern UART_BUFF_T TxBuff;
extern UART_BUFF_T RxBuff;

#define RS485_ENABLED		0
//#define TX_INTERRUPT		0		/* 0 if TX uses polling, 1 interrupt driven. */
#define TX_INTERRUPT		1		/* 0 if TX uses polling, 1 interrupt driven. */
#define MODEM_TEST			0

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE_UART		0x40

/* RS485 mode definition. */
#define RS485_NMMEN		(0x1<<0)
#define RS485_RXDIS		(0x1<<1)
#define RS485_AADEN		(0x1<<2)
#define RS485_SEL		(0x1<<3)
#define RS485_DCTRL		(0x1<<4)
#define RS485_OINV		(0x1<<5)

void UART_Config_Request(uint32_t Baudrate);
void UART_IRQHandler(void);

void UART_printf(const char *format, ...);
//////////////////////////////////////////////////////////////////////////////
#define _UART_SIDECHAN	1

void uart0_putc (uint8_t);
uint8_t uart0_getc (void);

#if _UART_SIDECHAN
extern volatile uint8_t UartCmd;
#endif

#endif /* end __UART_H */
