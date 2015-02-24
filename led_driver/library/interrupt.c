#include "interrupt.h"

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

void UART0_Handler(void)
{    
	uint32_t  status;
	DMA_control test_uart_rx_req;
    status = UDMA->CHIS;
	
	if((status & (0x01 << 8)) == (0x01 << 8)){
		//dma_channel_request(30);
		//display_buffer = (display_buffer == buffer_one) ? buffer_two : buffer_one;
		//destination = (display_buffer == buffer_one) ? &(buffer_two[1].blue) : &(buffer_one[1].blue);
		
		switch_buffers();
		
		dma_channel_enable(8);
		test_uart_rx_req.source = (void*) &(UART0->DR);
		test_uart_rx_req.destination = &(frame_buffer[1].blue);
		test_uart_rx_req.control = (DMA_DSTINC_BYTE | \
		DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
		DMA_ARBSIZE_4 | (5U << 4) | DMA_XFERMODE_BASIC);
	
		dma_primary_control_structure_set(8, &test_uart_rx_req);
		
		UDMA->CHIS |= (0x01 << 30); // Clear
	}
}
