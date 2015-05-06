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
			/*DMA_control touch_input_rx_req;
			uint32_t  status, *temp;
			status = UDMA->CHIS;
			
			//if((status & (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)){		
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
			//}*/
			uint8_t data;
			while ((UART6->FR & 0x10) != 0x10) {
				data = UART6->DR;
				add_byte_top(data);
			}
		}
		
		void UART7_Handler(void)
		{
			/*DMA_control touch_input_rx_req;
			uint32_t  status, *temp;
			status = UDMA->CHIS;
			
			//if((status & (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)){		
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
			//}*/
			
			uint8_t data;
			while ((UART7->FR & 0x10) != 0x10) {
				data = UART7->DR;
				add_byte_bottom(data);
			}
		}
	#endif
	
#else
		
	void GPIOA_Handler(void)
	{
		uint8_t status = PORT_A->RIS;
		if((status & BUTTON_CENTER) == BUTTON_CENTER) {
			center_pressed = true;
		} else if ((status & BUTTON_LEFT) == BUTTON_LEFT) {
			left_pressed = true;
		}
		
		PORT_A->ICR |= (BUTTON_LEFT | BUTTON_CENTER);
		xTaskResumeFromISR(hypervisor_task);
	}
	
	void GPIOC_Handler(void)
	{
		uint8_t status = PORT_C->RIS;
		if((status & BUTTON_DOWN) == BUTTON_DOWN) {
			down_pressed = true;
		} else if ((status & BUTTON_UP) == BUTTON_UP) {
			up_pressed = true;
		}
		
		PORT_C->ICR |= (BUTTON_DOWN | BUTTON_UP);
		xTaskResumeFromISR(hypervisor_task);
	}
	
	void GPIOE_Handler(void)
	{
		uint8_t status = PORT_E->RIS;
		if((status & BUTTON_RIGHT) == BUTTON_RIGHT) {
			right_pressed = true;
		} 
		
		PORT_E->ICR |= BUTTON_RIGHT;
		xTaskResumeFromISR(hypervisor_task);
	}
		
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
	
	void SSI0_Handler(void)
	{
		uint32_t  status;
		DMA_control lcd_push_req;
		status = UDMA->CHIS;
		
		if((status & (0x01 << LCD_DMA_CHANNEL)) == (0x01 << LCD_DMA_CHANNEL)){		
			UDMA->CHIS |= (0x01 << LCD_DMA_CHANNEL); // Clear
										
			while((LCD_SPI->SR & 0x01) != 0x01)
				; // Wait for data to finish sending
			
			if(lcd_pushing_init) {
			
				lcd_pushing_init = false;
			} else if(lcd_pushing_menu) {
				if(lcd_current_page >= 7) {
					lcd_pushing_menu = false;
					return;
				}
				
				lcd_current_page++;
				lcd_set_page(lcd_current_page);
				lcd_set_column(0);
				
				while((LCD_SPI->SR & 0x01) != 0x01)
					; // Wait for commands to send
					
				lcd_set_data_mode();

				lcd_push_req.source = (void*) &(lcd_buffer_read[(lcd_current_page * LCD_ROW_SIZE) + LCD_ROW_SIZE_MINUS_ONE]);
				lcd_push_req.destination = (void*) &(LCD_SPI->DR);
				lcd_push_req.control = (DMA_DSTINC_NONE | \
				DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
				DMA_ARBSIZE_1 | (LCD_ROW_SIZE_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);

				dma_primary_control_structure_set(LCD_DMA_CHANNEL, &lcd_push_req);	

				dma_channel_enable(LCD_DMA_CHANNEL);
			}
		}
	}
	
	void UART6_Handler(void)
	{
		DMA_control touch_input_rx_req;
		uint32_t  status, *temp;
		status = UDMA->CHIS;
		
		//if((status & (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_TOP_DMA_CHANNEL)){		
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
		//}
	}
	
	void UART7_Handler(void)
	{
		DMA_control touch_input_rx_req;
		uint32_t  status, *temp;
		status = UDMA->CHIS;
		
		//if((status & (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)) == (0x01 << TOUCH_INPUT_BOTTOM_DMA_CHANNEL)){		
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
		//}
	}
#endif