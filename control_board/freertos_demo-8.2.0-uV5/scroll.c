#include "scroll.h"

void draw_letter(LetterMap *letter, uint8_t x_pos, Pixel fill)
{
	int i, j;
	
	for(i = 0; i < letter->width; i++) {
		if(i < 0)
			continue;
	
		if(i + x_pos >= PIXELS_WIDTH)
			return;
	
		for(j = 0; j < LETTER_HEIGHT; j++) {
			if((letter->map[i] & (0x01 << (7 - j))) != 0)
				frame_buffer[(j * PIXELS_WIDTH) + (i + x_pos)] = fill;
		}
	}
}

void draw_string(char* string, uint8_t x_pos, Pixel fill)
{
	uint8_t current_x_pos;

	clear_frame_buffer();
	
	current_x_pos = x_pos;
	while((*string != '\0') && (current_x_pos < PIXELS_WIDTH)) {
		draw_letter(&letters[(uint8_t) (*string - ' ')], current_x_pos, fill);
		current_x_pos += (letters[(uint8_t) (*string - ' ')].width + 1);
		string++;
	}
	
	render();
}

void task_scroll_animation(void *pvParameters)
{
	Pixel blue;
	blue.blue = 255;

	draw_string("Test!", 0, blue);
	
	while(1) {
		vTaskDelay(TICK_DELAY_30_FPS);
	}
}