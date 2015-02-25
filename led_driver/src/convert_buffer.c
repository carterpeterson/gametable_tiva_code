#include "convert_buffer.h"

#define ACTIVE_SECTIONS 	1
#define WIDTH_PIXELS 		32

typedef struct {
	uint8_t pin;
	uint8_t mapping[32];
} Display_section;

static Display_section section_map[8] =
{
	{0x04, {0,1,2,3,4,5,6,7,8,9,
			10,11,12,13,14,15,16,17,18,19,
			20,21,22,23,24,25,26,27,28,29,
			30,31}},
	{0x02, {0}},
	{0x04, {0}},
	{0x08, {0}},
	{0x10, {0}},
	{0x20, {0}},
	{0x40, {0}},
	{0x80, {0}}
};

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
