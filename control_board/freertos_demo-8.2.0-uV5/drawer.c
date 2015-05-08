#include "drawer.h"

void draw_image_map(int image_width, int image_height, int x_pos, int y_pos, const uint8_t *image)
{
	int i, j;
	Pixel p;
  
	for(i = 0; i < image_width; i++) {
		if(i + x_pos < 0)
		  continue;

		if(i + x_pos >= PIXELS_WIDTH)
		  return;

		for(j = 0; j < image_height; j++) {
			if(j + y_pos < 0)
				continue;
			
			if(j + y_pos >= PIXELS_HEIGHT)
				break;
			
			p.red = image[(i) * (image_height * COLOR_CHANNELS_RGBA) + (j) * COLOR_CHANNELS_RGBA];
			p.green = image[(i) * (image_height * COLOR_CHANNELS_RGBA) + (j) * COLOR_CHANNELS_RGBA + 1];
			p.blue = image[(i) * (image_height * COLOR_CHANNELS_RGBA) + (j) * COLOR_CHANNELS_RGBA + 2];
		
			set_pixel((i + x_pos), (j + y_pos), p);
		}
	}
}

void task_drawer(void *pvParameters)
{
	int current_x = 0;
	
	while(1) {
		clear_frame_buffer();
	
		draw_image_map(138, 8, current_x, 0, &MARIO_LEVEL_ONE_1[0][0][0]);
		draw_image_map(138, 8, current_x + 138, 0, &MARIO_LEVEL_ONE_1[0][0][0]);
		
		render();
		
		current_x--;
		if(current_x + (138 + 32) < 32)
			current_x = 0;
			
		vTaskDelay(150);
	}
}