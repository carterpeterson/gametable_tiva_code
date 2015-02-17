#ifndef __UART_H__
#define __UART_H__

// C
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// MCU
#include "TM4C123.h"

// Project
#include "config.h"
#include "interrupt.h"
#include "pc_buffer.h"

// UART specific defines
#define UART0_CGC	0x01
#define UART1_CGC	0x02
#define UART2_CGC	0x04
#define UART3_CGC	0x08
#define UART4_CGC	0x10
#define UART5_CGC	0x20
#define UART6_CGC	0x40
#define UART7_CGC	0x80

#define UART_IM_TXIM            0x00000020  // UART Transmit Interrupt Mask
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_IFLS_TX1_8         0x00000000  // TX FIFO <= 1/8 full

#define UART_CTL_ENABLE		0x0001
#define UART_CTL_TX_ENABLE	0x0100
#define UART_CTL_RX_ENABLE	0x0200

#define UART_CTL_WORD_LENGTH_5 0x00
#define UART_CTL_WORD_LENGTH_6 0x20
#define UART_CTL_WORD_LENGTH_7 0x40
#define UART_CTL_WORD_LENGTH_8 0x60

#define UART0_BUFFER_SIZE 80

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_FR register.
//
//*****************************************************************************
#define UART_FR_TXFE            0x00000080  // UART Transmit FIFO Empty
#define UART_FR_RXFF            0x00000040  // UART Receive FIFO Full
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY            0x00000008  // UART Busy
#define UART_FR_CTS             0x00000001  // Clear To Send

void uart_enable(uint8_t uarts);
void uart_configure(UART0_Type *uart, uint32_t baud_rate);
void uart_transmit(UART0_Type *uart, uint8_t data);
void uart_receive(UART0_Type *uart, uint8_t data);

// Functions needed for the ARM MicroLib stdio to work properly
int fputc(int c, FILE *stream);
int fgetc(FILE *stream);

#endif
