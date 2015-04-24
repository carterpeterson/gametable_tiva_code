#include "interrupt_routines.h"

#ifdef USING_SIMULATOR
	void UART0_Handler(void)
	{    
		uint32_t  status;
		Pixel *temp;
		DMA_control uart_rx_req;
		status = UDMA->CHIS;
		
		if((status & (0x01 << 8)) == (0x01 << 8)){		
			temp = write_buffer;
			write_buffer = read_buffer;
			read_buffer = temp;
			
			dma_channel_enable(8);
			uart_rx_req.source = (void*) &(UART0->DR);
			uart_rx_req.destination = &(write_buffer[TOTAL_PIXELS - 1].blue);
			uart_rx_req.control = (DMA_DSTINC_BYTE | \
			DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
			DMA_ARBSIZE_2 | (INPUT_BUFF_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
			dma_primary_control_structure_set(8, &uart_rx_req);

			
			UDMA->CHIS |= (0x01 << 8); // Clear
		
			convert_buffer();
		} else if((status & (0x01 << CAPSENSE_OUTPUT_DMA_CHANNEL)) == (0x01 << CAPSENSE_OUTPUT_DMA_CHANNEL)){
			pushing_uart = false;
			UDMA->CHIS |= (0x01 << CAPSENSE_OUTPUT_DMA_CHANNEL); // Clear
		}
	}
#else
	void UART2_Handler(void)
	{    
		uint32_t  status;
		Pixel *temp;
		DMA_control uart_rx_req;
		status = UDMA->CHIS;
		
		if((status & (0x01 << 0)) == (0x01 << 0)){		
			temp = write_buffer;
			write_buffer = read_buffer;
			read_buffer = temp;
			
			dma_channel_enable(0);
			uart_rx_req.source = (void*) &(UART2->DR);
			uart_rx_req.destination = &(write_buffer[TOTAL_PIXELS - 1].blue);
			uart_rx_req.control = (DMA_DSTINC_BYTE | \
			DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
			DMA_ARBSIZE_2 | (INPUT_BUFF_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);		
			dma_primary_control_structure_set(0, &uart_rx_req);
		
			UDMA->CHIS |= (0x01 << 0); // Clear
			
			convert_buffer();
		}
	}
	
	void UART7_Handler(void)
	{
		uint32_t  status;
		status = UDMA->CHIS;
		
		if((status & (0x01 << CAPSENSE_OUTPUT_DMA_CHANNEL)) == (0x01 << CAPSENSE_OUTPUT_DMA_CHANNEL)){
			pushing_uart = false;
			UDMA->CHIS |= (0x01 << CAPSENSE_OUTPUT_DMA_CHANNEL); // Clear
		}
	}
#endif