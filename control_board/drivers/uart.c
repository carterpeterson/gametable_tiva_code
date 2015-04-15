#include "uart.h"

void uart_clock_enable(uint8_t uarts)
{
	SYSCTL->RCGCUART |= uarts;
	
	while((SYSCTL->PRUART & uarts) != uarts)
		;	// Wait for periheral ready
}

void uart_channel_disable(UART0_Type *uart)
{
	uart->CTL &= ~UART_CTL_ENABLE;
}

void uart_channel_enable(UART0_Type *uart, uint32_t enable_flags)
{
	uart->CTL = enable_flags;
}

void uart_config_baud(UART0_Type *uart, uint32_t baud_rate)
{	
	// Calculate baud rate
	float brd = ((float) SYSTEM_CLOCK_RATE) / ((float) (16 * baud_rate));
	int fbrd = (int) (((brd - floor(brd)) * 64) + 0.5);	

	// Set baud rate
	uart->IBRD = (int) floor(brd);
	uart->FBRD = fbrd;
}

void uart_config_line_control(UART0_Type *uart, uint8_t line_control)
{
  uart->LCRH = line_control;
}

void uart_enable_interrupts(UART0_Type *uart, uint8_t priority)
{
	IRQn_Type interrupt_vector;
	
	switch((uint32_t) uart) {
	case(UART0_BASE):
		interrupt_vector = UART0_IRQn;
		break;
	case(UART1_BASE):
		interrupt_vector = UART1_IRQn;
		break;
	case(UART2_BASE):
		interrupt_vector = UART2_IRQn;
		break;
	case(UART3_BASE):
		interrupt_vector = UART3_IRQn;
		break;
	case(UART4_BASE):
		interrupt_vector = UART4_IRQn;
		break;
	case(UART5_BASE):
		interrupt_vector = UART5_IRQn;
		break;
	case(UART6_BASE):
		interrupt_vector = UART6_IRQn;
		break;
	case(UART7_BASE):
		interrupt_vector = UART7_IRQn;
		break;
	default:
		return;
	}
	
	
  // Set the priority to 1
  NVIC_SetPriority(interrupt_vector, priority);
  
  // Enable the NVIC
  NVIC_EnableIRQ(interrupt_vector);
}

void uart_config_dma(UART0_Type *uart, uint8_t dma_config)
{
	uart->DMACTL = dma_config;
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
