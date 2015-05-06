#include "touch_input.h"

#ifdef USING_SIMULATOR_TOUCH
	uint32_t touch_buffers[2][8];
	uint32_t *touch_buffer_read, *touch_buffer_write;
#else
	uint32_t touch_buffers[2][2][4];
	uint32_t *touch_buffer_top_read, *touch_buffer_bottom_read;
	uint32_t *touch_buffer_top_write, *touch_buffer_bottom_write;
	uint8_t current_byte_top, current_byte_bottom;
	bool first_byte_top, first_byte_bottom;
#endif

void init_touch_buffers(void)
{
	#ifdef USING_SIMULATOR_TOUCH
		touch_buffer_read = touch_buffers[0];
		touch_buffer_write = touch_buffers[1];
	#else
		touch_buffer_top_read = touch_buffers[0][0];
		touch_buffer_top_write = touch_buffers[0][1];
		touch_buffer_bottom_read = touch_buffers[1][0];
		touch_buffer_bottom_write = touch_buffers[1][1];
	#endif
}

void init_touch_input_gpio(void)
{
	#ifdef USING_SIMULATOR_TOUCH
		// Was setup during the led_display setup,
		// as this also will use UART0
	#else
		// Setup the GPIO for UART7 (bottom half of board)
		gpio_port_enable(PORT_D_CGC);
		gpio_digital_enable(PORT_D, (PIN_4 | PIN_5));
		gpio_pin_direction(PORT_D, DIRECTION_OUTPUT, PIN_5);
		gpio_pin_direction(PORT_D, DIRECTION_INPUT, PIN_4);
		gpio_alternate_function_enable(PORT_D, (PIN_4 | PIN_5));
		gpio_config_port_ctl(PORT_D, (PIN_4 | PIN_5), 1);
		
		// Setup GPIO for UART6	(top half of board)
		gpio_port_enable(PORT_E_CGC);
		gpio_digital_enable(PORT_E, (PIN_0 | PIN_1));
		gpio_pin_direction(PORT_E, DIRECTION_OUTPUT, PIN_1);
		gpio_pin_direction(PORT_E, DIRECTION_INPUT, PIN_0);
		gpio_alternate_function_enable(PORT_E, (PIN_0 | PIN_1));
		gpio_config_port_ctl(PORT_E, (PIN_0 | PIN_1), 1);
	#endif
}

void init_touch_input_dma(void)
{
	#ifdef USING_SIMULATOR_TOUCH
		DMA_control touch_input_rx_req;
		
		dma_enable();
		dma_channel_enable(TOUCH_INPUT_DMA_CHANNEL);
		dma_priority_set(TOUCH_INPUT_DMA_CHANNEL, DMA_PRIORITY_HIGH);
		dma_use_primary_control(TOUCH_INPUT_DMA_CHANNEL);
		dma_use_burst(TOUCH_INPUT_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
		dma_peripheral_request_mask_set(TOUCH_INPUT_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
		UDMA->CHMAP1 &= ~(0x0F);
		
		touch_input_rx_req.source = (void*) &(UART0->DR);
		touch_input_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_write[TOUCH_BUFFER_SIZE])) - 1);
		touch_input_rx_req.control = (DMA_DSTINC_BYTE | \
		DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
		DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES - 1) << 4) | DMA_XFERMODE_BASIC);
		
		dma_primary_control_structure_set(TOUCH_INPUT_DMA_CHANNEL, &touch_input_rx_req);
	#else
		DMA_control touch_input_top_rx_req;
		DMA_control touch_input_bottom_rx_req;
		
		// UART 6 for the top half of the board
		dma_enable();
		dma_channel_enable(TOUCH_INPUT_TOP_DMA_CHANNEL);
		dma_priority_set(TOUCH_INPUT_TOP_DMA_CHANNEL, DMA_PRIORITY_HIGH);
		dma_use_primary_control(TOUCH_INPUT_TOP_DMA_CHANNEL);
		dma_use_burst(TOUCH_INPUT_TOP_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
		dma_peripheral_request_mask_set(TOUCH_INPUT_TOP_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
		UDMA->CHMAP1 &= ~(0x0F00);
		UDMA->CHMAP1 |= 0x0200;
		touch_input_top_rx_req.source = (void*) &(UART6->DR);
		touch_input_top_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_top_write[TOUCH_BUFFER_SIZE])) - 1);
		touch_input_top_rx_req.control = (DMA_DSTINC_BYTE | \
		DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
		DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES) << 4) | DMA_XFERMODE_BASIC);
		dma_primary_control_structure_set(TOUCH_INPUT_TOP_DMA_CHANNEL, &touch_input_top_rx_req);
		
		// UART 7 for the bottom half of the board
		dma_channel_enable(TOUCH_INPUT_BOTTOM_DMA_CHANNEL);
		dma_priority_set(TOUCH_INPUT_BOTTOM_DMA_CHANNEL, DMA_PRIORITY_HIGH);
		dma_use_primary_control(TOUCH_INPUT_BOTTOM_DMA_CHANNEL);
		dma_use_burst(TOUCH_INPUT_BOTTOM_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
		dma_peripheral_request_mask_set(TOUCH_INPUT_BOTTOM_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
		UDMA->CHMAP2 &= ~(0x000F0000);
		UDMA->CHMAP2 |= 0x00020000;
		touch_input_bottom_rx_req.source = (void*) &(UART7->DR);
		touch_input_bottom_rx_req.destination = (void*) (((uint32_t) &(touch_buffer_bottom_write[TOUCH_BUFFER_SIZE])) - 1);
		touch_input_bottom_rx_req.control = (DMA_DSTINC_BYTE | \
		DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
		DMA_ARBSIZE_2 | ((TOUCH_BUFFER_SIZE_BYTES) << 4) | DMA_XFERMODE_BASIC);
		
		dma_primary_control_structure_set(TOUCH_INPUT_BOTTOM_DMA_CHANNEL, &touch_input_bottom_rx_req);
	#endif
}

void init_touch_input_uart(void)
{
	#ifdef USING_SIMULATOR_TOUCH
		uart_config_dma(UART0, UART_DMACTL_RX_EN);
		UART0->IFLS &= (~0xF0);
		UART0->IFLS |= 0x10;
	#else
		// Setup UART6 and make it do DMA requests
		uart_clock_enable(UART6_CGC);
		uart_channel_disable(UART6);
		uart_config_baud(UART6, UART_BAUDRATE);
		uart_config_line_control(UART6, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
		uart_config_dma(UART6, UART_DMACTL_RX_EN);
		uart_enable_interrupts(UART6, 2);
		//UART6->IM |= 0x10;
		UART6->IFLS &= (~0xF0);
		UART6->IFLS |= 0x10;
		uart_channel_enable(UART6, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
		
		// Setup UART7 and make it do DMA requests
		uart_clock_enable(UART7_CGC);
		uart_channel_disable(UART7);
		uart_config_baud(UART7, UART_BAUDRATE);
		uart_config_line_control(UART7, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
		uart_config_dma(UART7, UART_DMACTL_RX_EN);
		uart_enable_interrupts(UART7, 2);
		//UART7->IM |= 0x10;
		UART7->IFLS &= (~0xF0);
		UART7->IFLS |= 0x10;
		uart_channel_enable(UART7, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
	#endif
}

void init_touch_input(void)
{
	init_touch_buffers();
	init_touch_input_gpio();
	init_touch_input_dma();
	init_touch_input_uart();
	/*first_byte_top = true;
	current_byte_top = 0;
	first_byte_bottom = true;
	current_byte_bottom = 0;*/
}

void add_byte_top(uint8_t byte)
{
	uint32_t* temp;
	uint8_t i;

	if(first_byte_top) {
		first_byte_top = false;
		return;
	}
	
	touch_buffer_top_write[(current_byte_top / 4)] |= (byte << (current_byte_top % 4));
	current_byte_top++;
	
	if(current_byte_top == 16) {
		temp = touch_buffer_top_read;
		touch_buffer_top_read = touch_buffer_top_write;
		touch_buffer_top_write = temp;
		
		for(i = 0; i < 4; i++) {
			touch_buffer_top_write[i] = 0;
		}
	}
}

void add_byte_bottom(uint8_t byte)
{
	uint32_t* temp;
	uint8_t i;

	if(first_byte_bottom) {
		first_byte_bottom = false;
		return;
	}
	
	touch_buffer_bottom_write[(current_byte_bottom / 4)] |= (byte << (current_byte_bottom % 4));
	current_byte_bottom++;
	
	if(current_byte_bottom == 16) {
		temp = touch_buffer_bottom_read;
		touch_buffer_bottom_read = touch_buffer_bottom_write;
		touch_buffer_bottom_write = temp;
		
		for(i = 0; i < 4; i++) {
			touch_buffer_bottom_write[i] = 0;
		}
	}
}

bool is_pixel_touched(uint8_t i, uint8_t j)
{
	#ifdef USING_SIMULATOR_TOUCH
		if ((touch_buffer_read[j] & (0x01 << (31 - i))) != 0) 
			return true;
	#else
		if(j <= 3) {
			if ((touch_buffer_top_read[j] & (0x01 << (31 - i))) != 0) 
				return true;
		} else {
			if ((touch_buffer_bottom_read[j - 4] & (0x01 << (31 - i))) != 0) 
				return true;
		}
	#endif
	
	return false;
}