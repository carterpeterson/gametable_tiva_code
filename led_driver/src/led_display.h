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
#include "input.h"
#include "app_config.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"

typedef struct {
	uint8_t pin;
	uint8_t mapping[32];
} Display_section;

static Display_section section_map[8] =
{
	{0x01, {0,1,2,3,4,5,6,7,8,9,
			10,11,12,13,14,15,16,17,18,19,
			20,21,22,23,24,25,26,27,28,29,
			30,31}},
	{0x02, {0}},
	{0x04, {0}},
	{0x08, {0}},
	{0x10, {0}},
	{0x20, {0}},
	{0x40, {0}},
	{0x80, {0}}
};

extern uint8_t *frame_buffer;		// The buffer being modified

void init_led_display(void);
void switch_buffers(void);
void convert_buffer(void);

#endif
