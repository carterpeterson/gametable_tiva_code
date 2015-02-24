#include "led_display.h"

uint8_t buffers[2][768];
uint8_t *display_buffer, *frame_buffer; // The buffer being output via DMA

extern void push_buffer(uint8_t* buffer, uint32_t size);

void init_screen_buffers(void)
{
	display_buffer = buffers[0];
	frame_buffer = buffers[1];
}

void switch_buffers(void)
{
	uint8_t* temp = frame_buffer;
	frame_buffer = display_buffer;
	display_buffer = temp;
	
	push_buffer(display_buffer, 3);
}
