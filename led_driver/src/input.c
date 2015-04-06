#include "input.h"

Pixel *read_buffer, *write_buffer;
Pixel input_buffers[2][STRING_LENGTH_BITS];

void dma_init(void)
{
	DMA_control test_uart_rx_req;
	
	dma_enable();
	dma_channel_enable(8);
	dma_priority_set(8, DMA_PRIORITY_HIGH);
	dma_use_primary_control(8);
	dma_use_burst(8, DMA_USE_BURST_AND_SINGLE);
	dma_peripheral_request_mask_set(8, DMA_REQ_MASK_PERIPHERAL);
	UDMA->CHMAP1 &= ~(0x0F);
	
	test_uart_rx_req.source = (void*) &(UART0->DR);
	test_uart_rx_req.destination = &(write_buffer[STRING_LENGTH_PIXEL - 1].blue);
	test_uart_rx_req.control = (DMA_DSTINC_BYTE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_4 | (INPUT_BUFF_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
	
	dma_primary_control_structure_set(8, &test_uart_rx_req);
	
	printf("\n\r-dma-\n\r");
}

void init_input_gpio(void)
{
	// Enable the UART0 GPIO to recieve frame buffer / debug output enable
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_0 | PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, (PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_INPUT, PIN_0);
	gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
	gpio_config_port_ctl(PORT_A, (PIN_0 | PIN_1), 1);
}

void init_input_uart(void)
{
	uart_clock_enable(UART0_CGC);
	uart_channel_disable(UART0);
	uart_config_baud(UART0, 460800);
	uart_config_line_control(UART0, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
	uart_config_dma(UART0, UART_DMACTL_RX_EN);
	uart_enable_interrupts(UART0, 1);
	uart_channel_enable(UART0, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
}

void init_input(void)
{
	write_buffer = input_buffers[0];
	read_buffer = input_buffers[1];
	
	init_input_gpio();
	init_input_uart();
	dma_init();
}
