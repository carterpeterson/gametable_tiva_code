#include "uart.h"

void uart_enable(uint8_t uarts)
{
	SYSCTL->RCGCUART |= uarts;
	
	while((SYSCTL->PRUART & uarts) != uarts)
		;	// Wait for periheral ready
}


void uart_configure(UART0_Type *uart, uint32_t baud_rate)
{	
	// Calculate baud rate
	double brd = SYSTEM_CLOCK_RATE / (16 * baud_rate);
	int fbrd = (int) (((brd - floor(brd)) * 64) + 0.5);	
	
	uart->CTL &= ~UART_CTL_ENABLE;	// Disable UART

	// Set baud rate
	uart->IBRD = 43;//27;//(int) floor(brd);
	uart->FBRD = 26;//9;//fbrd;
	
	// Line control settings
	uart->LCRH = UART_CTL_WORD_LENGTH_8;
	
	// Renable the UART
	uart->CTL = UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE;
}
