#include "interrupt.h"

extern PC_Buffer UART0_Tx_Buffer;

void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

//*****************************************************************************
// Tx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Tx_Flow(PC_Buffer *tx_buffer)
{
      char c;
  
        // Check to see if we have any data in the circular queue
				while(!pc_buffer_empty(tx_buffer) && (UART0->FR & UART_FR_TXFF)!= UART_FR_TXFF){
            // Move data from the circuljkar queue to the hardware FIFO
            // until the hardware FIFO is full or the circular buffer
            // is empty.
						pc_buffer_remove(tx_buffer, &c);
						UART0->DR = c;
					
				}
            // Any data in the hardware FIFO will continue to be transmitted
            // but the TX empty interrupt will not be enabled since there
            // is no data in the circular buffer.

            // Disable the TX interrupts.
				if(pc_buffer_empty(tx_buffer))
					UART0->IM &=  ~(UART_IM_TXIM);
        
        // Clear the TX interrupt so it can trigger again when the hardware
        // FIFO is empty
				UART0->ICR |=0x20;
}

void UART0_Handler(void)
{    
	uint32_t  status;

    // Check to see if RXMIS or RTMIS is active
    status = UART0->MIS;

    // Check the TX interrupts
    if ( status & UART_MIS_TXMIS )
    {
      UART0_Tx_Flow(&UART0_Tx_Buffer);
    }
    return;
}
