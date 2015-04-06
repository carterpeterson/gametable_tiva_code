#include "interrupt_routines.h"

void UART0_Handler(void)
{    
	uint32_t  status;
	Pixel *temp;
	DMA_control test_uart_rx_req;
    status = UDMA->CHIS;
	
	if((status & (0x01 << 8)) == (0x01 << 8)){		
		temp = write_buffer;
		write_buffer = read_buffer;
		read_buffer = temp;
		
		dma_channel_enable(8);
		test_uart_rx_req.source = (void*) &(UART0->DR);
		test_uart_rx_req.destination = &(write_buffer[STRING_LENGTH_PIXEL - 1].blue);
		test_uart_rx_req.control = (DMA_DSTINC_BYTE | \
		DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
		DMA_ARBSIZE_4 | (INPUT_BUFF_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
	
		dma_primary_control_structure_set(8, &test_uart_rx_req);
		
		UDMA->CHIS |= (0x01 << 8); // Clear
		
		printf("done\n\r");
		
		convert_buffer();
	}
}
