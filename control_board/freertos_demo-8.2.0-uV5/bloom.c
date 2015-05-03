#include "bloom.h"

static uint16_t current_color_base = 0;
static float MAX_RADIUS;

Pixel get_bloom_pixel(int i, int j)
{
  Pixel p;
  uint16_t current_color = current_color_base;

  current_color += (uint16_t) ((hypot((float) i, (float) j) / MAX_RADIUS) * FULL_RADIUS_STEP);
  current_color = current_color % 768;
  
  if(current_color < 256) {
    p.red = (255 - current_color);
    p.green = current_color;
    p.blue = 0;
  } else if(current_color < 512) {
    p.red = 0;
    p.green = (511 - current_color);
    p.blue = (current_color - 256);
  } else {
    p.red = (current_color - 512);
    p.green = 0;
    p.blue = 767 - current_color ;
  }

  return p;
}

void task_bloom_animation(void *pvParameters)
{
	int i, j, temp;
	Pixel black_pixel;
	TickType_t startup_time;
	MAX_RADIUS = hypot(BLOOM_RADIUS, BLOOM_RADIUS);

	black_pixel.red = 0;
	black_pixel.green = 0;
	black_pixel.blue = 0;
	
	startup_time = xTaskGetTickCount();

	while(1) {
		current_color_base += 1;
	
		if(current_color_base > (256 * 3))
		current_color_base = 0;

		for(i = 0; i < 32; i++) {
			temp = i;

			for(j = 0; j < 8; j++) {
				Pixel p;
				if((i % 6) < 3 && (j % 6) < 3) {
					p = get_bloom_pixel((i % 3), (j % 3));
				} else if ((i % 6) >= 3 && (j % 6) < 3) {
					p = get_bloom_pixel(3 - (i % 3), (j % 3));	
				} else if ((i % 6) < 3 && (j % 6) >= 3) {
					p = get_bloom_pixel((i % 3), 3 - (j % 3));	
				} else {
					p = get_bloom_pixel(3 - (i % 3), 3 - (j % 3));	
				}

				if(is_pixel_touched(i, j)) {
					set_pixel(i, j, p);
				} else {
					set_pixel(i, j, black_pixel);
				}
			}
			i = temp;
		}

		render();
		vTaskDelayUntil(&startup_time, TICK_DELAY_30_FPS);
		/*for(i = 0; i < 250000; i++) {
			// Wait
		}*/
	}
}