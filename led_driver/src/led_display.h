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
	{0x01, {15,31,30,14,13,29,28,12,
			11,27,26,10,9,25,24,8,7,
			23,22,6,5,21,20,4,3,19,
			18,2,1,17,16,0}},
	{0x02, {15,31,30,14,13,29,28,12,
			11,27,26,10,9,25,24,8,7,
			23,22,6,5,21,20,4,3,19,
			18,2,1,17,16,0}},
	{0x04, {15,31,30,14,13,29,28,12,
			11,27,26,10,9,25,24,8,7,
			23,22,6,5,21,20,4,3,19,
			18,2,1,17,16,0}},
	{0x08, {15,31,30,14,13,29,28,12,
			11,27,26,10,9,25,24,8,7,
			23,22,6,5,21,20,4,3,19,
			18,2,1,17,16,0}},
	{0x10, {16,0,1,17,18,2,3,19,20,
			4,5,21,22,6,7,23,24,8,
			9,25,26,10,11,27,28,12,
			13,29,30,14,15,31}},
	{0x20, {16,0,1,17,18,2,3,19,20,
			4,5,21,22,6,7,23,24,8,
			9,25,26,10,11,27,28,12,
			13,29,30,14,15,31}},
	{0x40, {16,0,1,17,18,2,3,19,20,
			4,5,21,22,6,7,23,24,8,
			9,25,26,10,11,27,28,12,
			13,29,30,14,15,31}},
	{0x80, {16,0,1,17,18,2,3,19,20,
			4,5,21,22,6,7,23,24,8,
			9,25,26,10,11,27,28,12,
			13,29,30,14,15,31}}
};

extern uint8_t *frame_buffer;		// The buffer being modified

void init_led_display(void);
void switch_buffers(void);
void convert_buffer(void);

#endif
