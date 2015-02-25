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
	
	printf("\n\r--start--\n\r");
}

void init_input(void)
{
	write_buffer = input_buffers[0];
	read_buffer = input_buffers[1];
	
	uart_enable(UART0_CGC);
	uart_configure(UART0, 500000);
	
	dma_init();
}
