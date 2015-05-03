#include "plot_animation.h"

// h is in radians
Pixel hsv_to_pixel(float h, float s, float v)
{
  Pixel return_pixel;
  float chroma, h_prime, x; // These are from wikipedia formula
  
  chroma = v * s;
  h_prime = fmodf(h,(2 * M_PI)) / (M_PI / 6);
  x = chroma * (1 - fabsf(fmodf(h_prime,2) - 1));

  if (h_prime < 1) {
    return_pixel.red = chroma * COLOR_SCALAR;
    return_pixel.green = x * COLOR_SCALAR;
    return_pixel.blue = 0;
  } else if (h_prime < 2) {
    return_pixel.red = x * COLOR_SCALAR;
    return_pixel.green = chroma * COLOR_SCALAR;
    return_pixel.blue = 0;
  } else if (h_prime < 3) {
    return_pixel.red = 0;
    return_pixel.green = chroma * COLOR_SCALAR;
    return_pixel.blue = x * COLOR_SCALAR;
  } else if (h_prime < 4) {
    return_pixel.red = 0;
    return_pixel.green = x * COLOR_SCALAR;
    return_pixel.blue = chroma * COLOR_SCALAR;
  } else if (h_prime < 5) {
    return_pixel.red = x * COLOR_SCALAR;
    return_pixel.green = 0;
    return_pixel.blue = chroma * COLOR_SCALAR;
  } else {
    return_pixel.red = chroma * COLOR_SCALAR;
    return_pixel.green = 0;
    return_pixel.blue = x * COLOR_SCALAR;
  }

  return return_pixel;
}

// rotation matrix

float rotate_function(void* orig_plot_fn, float radians_rot, float x, float y)
{
  float (*plot_fn)(float,float) = orig_plot_fn;
  return (*plot_fn)((x*cos(radians_rot) - y*sin(radians_rot)),
		    (x*sin(radians_rot) + y*cos(radians_rot)));
}

float sinxy_squared(float x, float y)
{
  return sin(x*y) * sin(x*y);
}


void task_plot_animation(void *pvParameters)
{
  int i, j;
  float scaled_i, scaled_j, wobble_x, wobble_y;
  float rotation_amount = 0;
  float zoom_amount = 0;
  float color_base = COLOR_BASE * 2 * M_PI;
  void* plot_fn = &sinxy_squared;
  wobble_x = 0;
  wobble_y = 0;

  
  while(1) {
    for(i = 0; i < PIXELS_WIDTH; i++) {
      for(j = 0; j < PIXELS_HEIGHT; j++) {
	scaled_i = ((float) (i - (BASE_POSITION_X + WOBBLE_DISTANCE_X * sinf(wobble_x)))) / ((SIZE_SCALE_X * ZOOM_BASE) + SIZE_SCALE_X * ZOOM_RANGE * sinf(zoom_amount));
	scaled_j = ((float) (j - (BASE_POSITION_Y + WOBBLE_DISTANCE_Y * sinf(wobble_y)))) / ((SIZE_SCALE_Y * ZOOM_BASE) + SIZE_SCALE_Y * ZOOM_RANGE * sinf(zoom_amount));
	
	set_pixel(i, j, hsv_to_pixel(color_base + COLOR_RANGE * 2 * M_PI * rotate_function(plot_fn,
									     rotation_amount,
									     scaled_i, scaled_j),
				     1, 1));
      }
    }

    render();

    wobble_x = fmodf((wobble_x + (M_PI * (((float) WOBBLE_X_SPEED) / 180))), (2 * M_PI));
    wobble_y = fmodf((wobble_y + (M_PI * (((float) WOBBLE_Y_SPEED) / 180))), (2 * M_PI));
    color_base = fmodf((color_base + (M_PI * (((float) COLOR_SCROLL_SPEED) / 180))), (2 * M_PI));
    zoom_amount = fmodf((zoom_amount + (M_PI * (((float) ZOOM_SPEED) / 180))), (2 * M_PI));
    rotation_amount += (((float) ROTATION_SPEED) / 180) * M_PI;
    
    vTaskDelay(TICK_DELAY_30_FPS);
  }
}
