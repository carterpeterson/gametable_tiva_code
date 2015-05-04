#include "led_display.h"

Pixel buffers[2][256];
Pixel *display_buffer, *frame_buffer; // The buffer being output via DMA
bool pushing_frame = false;

extern void push_buffer(uint8_t* buffer, uint32_t size);

void init_screen_buffers(void)
{
	display_buffer = buffers[1];
	frame_buffer = buffers[0];
}

void transfer_frame_buffer(void)
{
	DMA_control frame_buffer_tx_req;

	frame_buffer_tx_req.source = &(display_buffer[255].blue);
	#ifdef USING_SIMULATOR
		frame_buffer_tx_req.destination = (void*) &(UART0->DR);
	#else
		frame_buffer_tx_req.destination = (void*) &(UART2->DR);
	#endif
	frame_buffer_tx_req.control = (DMA_DSTINC_NONE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_2 | (FRAME_BUFF_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
		
	dma_primary_control_structure_set(FRAME_OUTPUT_DMA_CHANNEL, &frame_buffer_tx_req);	
		
	dma_channel_enable(FRAME_OUTPUT_DMA_CHANNEL);
}

void clear_frame_buffer(void)
{
	int i = 0;
	for(; i < 256; i++) {
		frame_buffer[i].red = 0;
		frame_buffer[i].green = 0;
		frame_buffer[i].blue = 0;
	}
}

void render(void)
{
	Pixel* temp = frame_buffer;
	frame_buffer = display_buffer;
	display_buffer = temp;

	if(pushing_frame == false) {
		pushing_frame = true;
		transfer_frame_buffer();
	}
}

void init_led_display_dma(void)
{
	dma_enable();
	
	dma_priority_set(FRAME_OUTPUT_DMA_CHANNEL, DMA_PRIORITY_HIGH);
	dma_use_primary_control(FRAME_OUTPUT_DMA_CHANNEL);
	dma_use_burst(FRAME_OUTPUT_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
	dma_peripheral_request_mask_set(FRAME_OUTPUT_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
	
	#ifdef USING_SIMULATOR
		UDMA->CHMAP1 &= ~(0xF0);
	#else
		UDMA->CHMAP0 &= ~(0xF0);
		UDMA->CHMAP0 |= (0x10);
	#endif
}

void init_led_display_gpio(void)
{
	#ifdef USING_SIMULATOR
		// Enable the UART0 GPIO to recieve frame buffer / debug output enable
		gpio_port_enable(PORT_A_CGC);
		gpio_digital_enable(PORT_A, (PIN_0 | PIN_1));
		gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, PIN_1);
		gpio_pin_direction(PORT_A, DIRECTION_INPUT, PIN_0);
		gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
		gpio_config_port_ctl(PORT_A, (PIN_0 | PIN_1), 1);
	#else
		// Setup the GPIO for UART2
		gpio_port_enable(PORT_D_CGC);
		gpio_digital_enable(PORT_D, (PIN_6 | PIN_7));
		gpio_pin_direction(PORT_D, DIRECTION_OUTPUT, PIN_7);
		gpio_pin_direction(PORT_D, DIRECTION_INPUT, PIN_6);
		gpio_alternate_function_enable(PORT_D, (PIN_6 | PIN_7));
		gpio_config_port_ctl(PORT_D, (PIN_6 | PIN_7), 1);
	#endif
}

void init_led_display_uart(void)
{
	#ifdef USING_SIMULATOR
		uart_clock_enable(UART0_CGC);
		uart_channel_disable(UART0);
		uart_config_baud(UART0, UART_BAUDRATE);
		uart_config_line_control(UART0, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
		uart_config_dma(UART0, UART_DMACTL_TX_EN);
		uart_enable_interrupts(UART0, 3);
		UART0->IFLS &= (~0x0F);
		UART0->IFLS |= 0x03;
		uart_channel_enable(UART0, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
	#else
		uart_clock_enable(UART2_CGC);
		uart_channel_disable(UART2);
		uart_config_baud(UART2, UART_BAUDRATE);
		uart_config_line_control(UART2, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
		uart_config_dma(UART2, UART_DMACTL_TX_EN);
		uart_enable_interrupts(UART2, 3);
		UART2->IFLS = 0x03;
		uart_channel_enable(UART2, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
	#endif
}

void init_led_display(void)
{	
	init_screen_buffers();
	init_led_display_gpio();
	init_led_display_dma();
	init_led_display_uart();
}

void set_pixel(int i, int j, Pixel p)
{
	frame_buffer[j * WIDTH_PIXELS + i] = p;
}