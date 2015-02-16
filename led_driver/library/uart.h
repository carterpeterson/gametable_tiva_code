#ifndef __UART_H__
#define __UART_H__

// C
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// MCU
#include "TM4C123.h"

// Project
#include "config.h"

// UART specific defines
#define UART0_CGC	0x01
#define UART1_CGC	0x02
#define UART2_CGC	0x04
#define UART3_CGC	0x08
#define UART4_CGC	0x10
#define UART5_CGC	0x20
#define UART6_CGC	0x40
#define UART7_CGC	0x80

#define UART_CTL_ENABLE		0x0001
#define UART_CTL_TX_ENABLE	0x0100
#define UART_CTL_RX_ENABLE	0x0200

#define UART_CTL_WORD_LENGTH_5 0x00
#define UART_CTL_WORD_LENGTH_6 0x20
#define UART_CTL_WORD_LENGTH_7 0x40
#define UART_CTL_WORD_LENGTH_8 0x60

void uart_enable(uint8_t uarts);
void uart_configure(UART0_Type *uart, uint32_t baud_rate);

#endif
