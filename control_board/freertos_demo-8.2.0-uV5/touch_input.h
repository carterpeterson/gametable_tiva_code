#ifndef __H_TOUCH_INPUT__
#define __H_TOUCH_INPUT__

// C
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"
#include "system_TM4C123.h"

// Project
#include "app_config.h"
#include "../drivers/gpio.h"
#include "../drivers/dma.h"
#include "../drivers/uart.h"

#ifdef USING_SIMULATOR
	#define TOUCH_INPUT_DMA_CHANNEL 8
	#define TOUCH_BUFFER_SIZE 		8
	#define TOUCH_BUFFER_SIZE_BYTES	32
	extern uint32_t *touch_buffer_read, *touch_buffer_write;
#else
	#define TOUCH_INPUT_TOP_DMA_CHANNEL 	10
	#define TOUCH_INPUT_BOTTOM_DMA_CHANNEL 	20
	#define TOUCH_BUFFER_SIZE 		4
	#define TOUCH_BUFFER_SIZE_BYTES	16
	extern uint32_t *touch_buffer_top_read, *touch_buffer_bottom_read;
	extern uint32_t *touch_buffer_top_write, *touch_buffer_bottom_write;
#endif

void init_touch_input(void);
bool is_pixel_touched(uint8_t i, uint8_t j);

#endif
