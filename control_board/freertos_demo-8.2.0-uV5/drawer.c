#include "drawer.h"

void draw_image_map(int image_width, int image_height, int x_pos, int y_pos, const uint8_t *image)
{
	int i, j;
	Pixel p;
	
	clear_frame_buffer();
  
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
	
	render();
}

void task_drawer(void *pvParameters)
{
	draw_image_map(8, 24, 0, -8, &MARIO_LEVEL_ONE[0][0][0]);
	
	while(1) {
		vTaskDelay(TICK_DELAY_30_FPS);
	}
}