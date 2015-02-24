#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__

// C
#include <stdint.h>

// MCU

// Project

typedef struct {
	uint8_t red, green, blue;
} Pixel;

extern Pixel *frame_buffer;		// The buffer being modified

void init_buffers(void);
void switch_buffers(void);

#endif
