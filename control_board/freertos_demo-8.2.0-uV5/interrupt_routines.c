#include "interrupt_routines.h"

#ifdef USING_SIMULATOR
	void UART0_Handler(void)
	{    
		DMA_control touch_input_rx_req;
		uint32_t  status, *temp;
		status = UDMA->CHIS;
		
		if((status & (0x01 << FRAME_OUTPUT_DMA_CHANNEL)) == (0x01 << FRAME_OUTPUT_DMA_CHANNEL)){		
			UDMA->CHIS |= (0x01 << FRAME_OUTPUT_DMA_CHANNEL); // Clear
			pushing_frame = false;
		} 
		#ifdef USING_SIMULATOR_TOUCH
		else if((status & (0x01 << TOUCH_INPUT_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_DMA_CHANNEL)){
			UDMA->CHIS |= (0x01 << TOUCH_INPUT_DMA_CHANNEL); // Clear
			
			// Swap the buffers
			temp = touch_buffer_write;
			touch_buffer_write = touch_buffer_read;
			touch_buffer_read = temp;
			
			// Reset the DMA receive request
			touch_input_rx_req.source = (void*) &(UART0->DR);
			touch_input_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_write[TOUCH_BUFFER_SIZE])) - 1);
			touch_input_rx_req.control = (DMA_DSTINC_BYTE | \
			DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
			DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES - 1) << 4) | DMA_XFERMODE_BASIC);
			
			dma_primary_control_structure_set(TOUCH_INPUT_DMA_CHANNEL, &touch_input_rx_req);
			dma_channel_enable(TOUCH_INPUT_DMA_CHANNEL);
		}
		#endif
	}
	
	#ifndef USING_SIMULATOR_TOUCH
		void UART6_Handler(void)
		{
			DMA_control touch_input_rx_req;
			uint32_t  status, *temp;
			status = UDMA->CHIS;
			
			if((status & (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)){		
				UDMA->CHIS |= (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL); // Clear

				temp = touch_buffer_top_write;
				touch_buffer_top_write = touch_buffer_top_read;
				touch_buffer_top_read = temp;

				touch_input_rx_req.source = (void*) &(UART6->DR);
				touch_input_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_top_write[TOUCH_BUFFER_SIZE])) - 1);
				touch_input_rx_req.control = (DMA_DSTINC_BYTE | \
				DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
				DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES - 1) << 4) | DMA_XFERMODE_BASIC);
				dma_primary_control_structure_set(TOUCH_INPUT_TOP_DMA_CHANNEL, &touch_input_rx_req);
				dma_channel_enable(TOUCH_INPUT_TOP_DMA_CHANNEL);
			}
		}
		
		void UART7_Handler(void)
		{
			DMA_control touch_input_rx_req;
			uint32_t  status, *temp;
			status = UDMA->CHIS;
			
			if((status & (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)){		
				UDMA->CHIS |= (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL); // Clear

				temp = touch_buffer_bottom_write;
				touch_buffer_bottom_write = touch_buffer_bottom_read;
				touch_buffer_bottom_read = temp;

				touch_input_rx_req.source = (void*) &(UART7->DR);
				touch_input_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_bottom_write[TOUCH_BUFFER_SIZE])) - 1);
				touch_input_rx_req.control = (DMA_DSTINC_BYTE | \
				DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
				DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES - 1) << 4) | DMA_XFERMODE_BASIC);
				dma_primary_control_structure_set(TOUCH_INPUT_BOTTOM_DMA_CHANNEL, &touch_input_rx_req);
				dma_channel_enable(TOUCH_INPUT_BOTTOM_DMA_CHANNEL);
			}
		}
	#endif
	
#else
	void HardFault_Handler(void)
	{
		while(1) {
			// iz dead
		}
	}

	void UART2_Handler(void)
	{    
		uint32_t  status;
		status = UDMA->CHIS;
		
		if((status & (0x01 << FRAME_OUTPUT_DMA_CHANNEL)) == (0x01 << FRAME_OUTPUT_DMA_CHANNEL)){		
			UDMA->CHIS |= (0x01 << FRAME_OUTPUT_DMA_CHANNEL); // Clear
			pushing_frame = false;
		}
	}
	
	void UART6_Handler(void)
	{
		DMA_control touch_input_rx_req;
		uint32_t  status, *temp;
		status = UDMA->CHIS;
		
		if((status & (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)){		
			UDMA->CHIS |= (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL); // Clear

			temp = touch_buffer_top_write;
			touch_buffer_top_write = touch_buffer_top_read;
			touch_buffer_top_read = temp;

			touch_input_rx_req.source = (void*) &(UART6->DR);
			touch_input_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_top_write[TOUCH_BUFFER_SIZE])) - 1);
			touch_input_rx_req.control = (DMA_DSTINC_BYTE | \
			DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
			DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES - 1) << 4) | DMA_XFERMODE_BASIC);
			dma_primary_control_structure_set(TOUCH_INPUT_TOP_DMA_CHANNEL, &touch_input_rx_req);
			dma_channel_enable(TOUCH_INPUT_TOP_DMA_CHANNEL);
		}
	}
	
	void UART7_Handler(void)
	{
		DMA_control touch_input_rx_req;
		uint32_t  status, *temp;
		status = UDMA->CHIS;
		
		if((status & (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)){		
			UDMA->CHIS |= (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL); // Clear

			temp = touch_buffer_bottom_write;
			touch_buffer_bottom_write = touch_buffer_bottom_read;
			touch_buffer_bottom_read = temp;

			touch_input_rx_req.source = (void*) &(UART7->DR);
			touch_input_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_bottom_write[TOUCH_BUFFER_SIZE])) - 1);
			touch_input_rx_req.control = (DMA_DSTINC_BYTE | \
			DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
			DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES - 1) << 4) | DMA_XFERMODE_BASIC);
			dma_primary_control_structure_set(TOUCH_INPUT_BOTTOM_DMA_CHANNEL, &touch_input_rx_req);
			dma_channel_enable(TOUCH_INPUT_BOTTOM_DMA_CHANNEL);
		}
	}
#endif