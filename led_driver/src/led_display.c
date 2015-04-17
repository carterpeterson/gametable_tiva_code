#include "led_display.h"

uint8_t buffers[2][768];
uint8_t *display_buffer, *frame_buffer; // The buffer being output via DMA
static uint8_t gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

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
	gpio_digital_enable(PORT_B, (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4 | PIN_5 | PIN_6 | PIN_7));
	gpio_pin_direction(PORT_B, DIRECTION_OUTPUT, (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4 | PIN_5 | PIN_6 | PIN_7));
	
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
				if(((gamma[read_buffer[y * WIDTH_PIXELS + x].green] << bit) & 0x0080) == 0x0080) {
					frame_buffer[(p*24) + bit] |= current_section.pin;
				} else {
					frame_buffer[(p*24) + bit] &= ~current_section.pin;
				}
				
				if(((gamma[read_buffer[y * WIDTH_PIXELS + x].red] << bit) & 0x0080) == 0x0080) {
					frame_buffer[(p*24) + bit + 8] |= current_section.pin;
				} else {
					frame_buffer[(p*24) + bit + 8] &= ~current_section.pin;
				}
				
				if(((gamma[read_buffer[y * WIDTH_PIXELS + x].blue] << bit) & 0x0080) == 0x0080) {
					frame_buffer[(p*24) + bit + 16] |= current_section.pin;
				} else {
					frame_buffer[(p*24) + bit + 16] &= ~current_section.pin;
				}
			}
		}
	}
	
	switch_buffers();
}