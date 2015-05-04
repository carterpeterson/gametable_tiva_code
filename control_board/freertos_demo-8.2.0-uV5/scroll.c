#include "scroll.h"

void draw_letter(LetterMap *letter, int x_pos, Pixel fill)
{
  int i, j;
  
  for(i = 0; i < letter->width; i++) {
    if(i + x_pos < 0)
      continue;
    
    if(i + x_pos >= PIXELS_WIDTH)
      return;
    
    for(j = 0; j < LETTER_HEIGHT; j++) {
      if((letter->map[i] & (0x01 << (7 - j))) != 0)
		frame_buffer[(j * PIXELS_WIDTH) + (i + x_pos)] = fill;
    }
  }
}

void draw_string(char* string, int x_pos, Pixel fill)
{
	int current_x_pos;

	clear_frame_buffer();
	
	current_x_pos = x_pos;
	while((*string != '\0') && (current_x_pos < PIXELS_WIDTH)) {
		draw_letter(&letters[(uint8_t) (*string - ' ')], current_x_pos, fill);
		current_x_pos += (letters[(uint8_t) (*string - ' ')].width + 1);
		string++;
	}
	
	render();
}

int string_width(char *string)
{
  int width = 0;
  while(*string != '\0') {
    width += letters[(uint8_t) (*string - ' ')].width;
    string++;
    if(*string != '\0')
      width += 1;
  }
  return width;
}

void scroll_string(char* string, int step_delay_ms, Pixel fill)
{
  int current_x = 31;

  while(current_x > -(string_width(string))) {
    draw_string(string, current_x, fill);
    current_x--;
	vTaskDelay(step_delay_ms);
  }
}

void task_scroll_animation(void *pvParameters)
{
	Pixel blue;
	blue.blue = 255;
	
	while(1) {
	  scroll_string("Tobias the Table! Sponsored by XES", (3 * TICK_DELAY_30_FPS), blue);
	}
}