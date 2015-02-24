#include "led_display.h"

Pixel buffers[2][768];
Pixel *display_buffer, *frame_buffer; // The buffer being output via DMA

extern void push_buffer(Pixel* buffer);

void init_buffers(void)
{
	display_buffer = buffers[0];
	frame_buffer = buffers[1];
}

void switch_buffers(void)
{
	Pixel* temp = frame_buffer;
	frame_buffer = display_buffer;
	display_buffer = temp;
	
	push_buffer(display_buffer);
}
