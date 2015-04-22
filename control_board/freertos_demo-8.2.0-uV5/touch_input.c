#include "touch_input.h"

#ifdef USING_SIMULATOR
	uint32_t touch_buffers[2][8];
	uint32_t *touch_buffer_read, *touch_buffer_write;
#else
	uint32_t touch_buffers[2][2][4];
	uint32_t *touch_buffer_top_read, *touch_buffer_bottom_read;
	uint32_t *touch_buffer_top_write, *touch_buffer_bottom_write;
#endif

void init_touch_buffers(void)
{
	#ifdef USING_SIMULATOR
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
	#ifdef USING_SIMULATOR
		// Was setup during the led_display setup,
		// as this also will use UART0
	#else
		// Pick a couple UART channels and set it up
	#endif
}

void init_touch_input_dma(void)
{
	#ifdef USING_SIMULATOR
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
	
	#endif
}

void init_touch_input_uart(void)
{
	#ifdef USING_SIMULATOR
		uart_config_dma(UART0, UART_DMACTL_RX_EN);
		UART0->IFLS &= (~0xF0);
		UART0->IFLS |= 0x10;
	#else
		// Setup the couple UARTS
	#endif
}

void init_touch_input(void)
{
	init_touch_buffers();
	init_touch_input_gpio();
	init_touch_input_dma();
	init_touch_input_uart();
}

bool is_pixel_touched(uint8_t i, uint8_t j)
{
	#ifdef USING_SIMULATOR
		if ((touch_buffer_read[j] & (0x01 << (31 - i))) != 0) 
			return true;
	#else
	
	#endif
	
	return false;
}