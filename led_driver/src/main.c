#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "TM4C123.h"
#include "system_TM4C123.h"

#include "../library/led_display.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"

Pixel *read_buffer, *write_buffer;
Pixel input_buffers[2][768];

void usleep(int microseconds)
{
		int i = 90000;
		while(i > 0)
			i--;
}

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
	test_uart_rx_req.destination = &(write_buffer[1].blue);
	test_uart_rx_req.control = (DMA_DSTINC_BYTE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_4 | (5U << 4) | DMA_XFERMODE_BASIC);
	
	dma_primary_control_structure_set(8, &test_uart_rx_req);
	
	printf("\n\r--start--\n\r");
}


int main(void)
{	
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_0 | PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, (PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_INPUT, PIN_0);
	gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
	PORT_A->PCTL = 0x0011;
	
	write_buffer = input_buffers[0];
	read_buffer = input_buffers[1];
	
	init_screen_buffers();
	
	//timer_init(TIMER0);
	//timer_periodic_enable(TIMER0);
	
	uart_enable(UART0_CGC);
	uart_configure(UART0, 500000);
	
	dma_init();
	
	while(1) {
		//switch_buffers();
	}
	
}
