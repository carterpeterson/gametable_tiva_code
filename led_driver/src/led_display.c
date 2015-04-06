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
	
	push_buffer(display_buffer, STRING_LENGTH_PIXEL);
}

void init_led_display_gpio(void)
{
	// Enable the pins for LED signal output
	gpio_port_enable(PORT_B_CGC);
	gpio_digital_enable(PORT_B, (PIN_0));
	gpio_pin_direction(PORT_B, DIRECTION_OUTPUT, PIN_0);
	
	// Change the DIR on the 3v->5v pixel signal buffer to output
	gpio_port_enable(PORT_D_CGC);
	gpio_digital_enable(PORT_D, (PIN_2));
	gpio_pin_direction(PORT_D, DIRECTION_OUTPUT, PIN_2);
	PORT_D->DATA = 0x04;	
}

void init_led_display(void)
{	
	init_led_display_gpio();
	init_screen_buffers();
}

void convert_buffer(void)
{
	int s;
	Display_section current_section;
	uint8_t x_off, y_off, x, y, p, map, bit; 
	
	for(s = 0; s < ACTIVE_SECTIONS; s++) {
		current_section = section_map[s];
		
		x_off = (s > 3) ? 16 : 0;
		y_off = (s % 4) * 2;

		for(p = 0; p < STRING_LENGTH_PIXEL; p++) {
			map = section_map[s].mapping[p];
			x = x_off + (map % 16);
			y = y_off + (map / 16);
			
			for(bit = 0; bit < 8; bit++) {
				if(((read_buffer[y * WIDTH_PIXELS + x].green << bit) & 0x0080) == 0x0080) {
					frame_buffer[(p*24) + bit] |= current_section.pin;
				} else {
					frame_buffer[(p*24) + bit] &= ~current_section.pin;
				}
				
				if(((read_buffer[y * WIDTH_PIXELS + x].red << bit) & 0x0080) == 0x0080) {
					frame_buffer[(p*24) + bit + 8] |= current_section.pin;
				} else {
					frame_buffer[(p*24) + bit + 8] &= ~current_section.pin;
				}
				
				if(((read_buffer[y * WIDTH_PIXELS + x].blue << bit) & 0x0080) == 0x0080) {
					frame_buffer[(p*24) + bit + 16] |= current_section.pin;
				} else {
					frame_buffer[(p*24) + bit + 16] &= ~current_section.pin;
				}
			}
		}
	}
	
	switch_buffers();
}