#include "uart.h"

PC_Buffer UART0_Rx_Buffer;
char UART0_Rx_Buffer_Array[UART0_BUFFER_SIZE];

PC_Buffer UART0_Tx_Buffer;
char UART0_Tx_Buffer_Array[UART0_BUFFER_SIZE];

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

    // Configure the FIFO Interrupt Levels
    UART0->IFLS = UART_IFLS_TX1_8;
    
    // Turn on the UART Interrupts  for Tx, Rx, and Rx Timeout
    UART0->IM = UART_IM_TXIM;
    
    // Set the priority to 1
    NVIC_SetPriority(UART0_IRQn, 1);
  
    // Enable the NVIC
    NVIC_EnableIRQ(UART0_IRQn);
    
    // Initialize the circular buffer
    pc_buffer_init(&UART0_Tx_Buffer,UART0_Tx_Buffer_Array, UART0_BUFFER_SIZE); 
    pc_buffer_init(&UART0_Rx_Buffer,UART0_Rx_Buffer_Array, UART0_BUFFER_SIZE); 
	
	// Renable the UART
	uart->CTL = UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE;
}

void uart_transmit(UART0_Type *uart, uint8_t data)
{
	// If there is sapce in the hardwere FIFO, and the circular
	// buffer is empty, send the data to the FIFO.	
	if(((uart->FR & UART_FR_TXFF) != UART_FR_TXFF) && pc_buffer_empty(&UART0_Tx_Buffer)) {
		uart->DR = data;
	} else {
		while(pc_buffer_full(&UART0_Tx_Buffer))
			; // Spin Wait
		
		DisableInterrupts();
		pc_buffer_add(&UART0_Tx_Buffer, data);
		EnableInterrupts();
	}

	// If you're in this function, you want to send data
	// so enable TX interrupts even if they are already enabled.
	uart->IM |= UART_IM_TXIM;
	
  return;	
}

void uart_receive(UART0_Type *uart, uint8_t data)
{
	
}

// Functions needed for the ARM MicroLib stdio to work properly

int fputc(int c, FILE* stream)
{
	if (stream != stdout) // bah! to stderr
	{
		return EOF;
	}

	uart_transmit(UART0, c);

	if (c == '\n')
		uart_transmit(UART0, '\r');

	return c;
}
int fgetc(FILE *stream)
{
	return 0;
}
