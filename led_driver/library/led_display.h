#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__

// C
#include <stdint.h>

// MCU

// Project

#define STRING_LENGTH_PIXEL 8

typedef struct {
	uint8_t red, green, blue;
} Pixel;

extern uint8_t *frame_buffer;		// The buffer being modified

void init_screen_buffers(void);
void switch_buffers(void);

#endif
