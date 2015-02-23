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
	float brd = ((float) SYSTEM_CLOCK_RATE) / ((float) (16 * baud_rate));
	int fbrd = (int) (((brd - floor(brd)) * 64) + 0.5);	
	
	uart->CTL &= ~UART_CTL_ENABLE;	// Disable UART

	// Set baud rate
	uart->IBRD = (int) floor(brd);
	uart->FBRD = fbrd;

	
	// Configure the Line Control for 8N1, FIFOs
    UART0->LCRH = UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN;

	UART0->DMACTL |= 0x01;	// Enable Receive DMA
    
    // Set the priority to 1
    NVIC_SetPriority(UART0_IRQn, 1);
  
    // Enable the NVIC
    NVIC_EnableIRQ(UART0_IRQn);
	
	// Renable the UART
	uart->CTL = UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE;
}

// Functions needed for the ARM MicroLib stdio to work properly

int fputc(int c, FILE* stream)
{
	UART0->DR = c;

	return c;
}
int fgetc(FILE *stream)
{
	return 0;
}
