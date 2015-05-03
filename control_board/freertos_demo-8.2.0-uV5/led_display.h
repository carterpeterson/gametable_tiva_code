#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__

// C
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"
#include "system_TM4C123.h"

// Including up here to avoid circular dependency in with input.h,
// currently input.h needs Pixel for its input buffer. I'll fix
// this circular dependancy later.
typedef struct {
	uint8_t red, green, blue;
} Pixel;

// Project
#include "app_config.h"
#include "../drivers/gpio.h"
#include "../drivers/timer.h"
#include "../drivers/dma.h"
#include "../drivers/uart.h"

#ifdef USING_SIMULATOR
	#define FRAME_OUTPUT_DMA_CHANNEL 9
#else
	#define FRAME_OUTPUT_DMA_CHANNEL 1
#endif

#define PIXELS_WIDTH 	32
#define PIXELS_HEIGHT 	8

extern Pixel *frame_buffer;		// The buffer being modified
extern bool pushing_frame;

void render(void);
void init_led_display(void);
void set_pixel(int i, int j, Pixel p);

#endif
