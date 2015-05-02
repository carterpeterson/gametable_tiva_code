#ifndef __BUTTON_INPUT_H__
#define __BUTTON_INPUT_H__

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
#include "../drivers/spi.h"
#include "../drivers/dma.h"
#include "hypervisor.h"
#include "lcd_graphics.h"
#include "tasks.h"

#define		BUTTON_UP			PIN_6	// on port C
#define		BUTTON_DOWN		PIN_7	// on port C
#define 	BUTTON_LEFT		PIN_7	// on port A
#define		BUTTON_CENTER	PIN_6	// on port A
#define		BUTTON_RIGHT	PIN_3	// on port E

void init_button_input(void);

extern bool left_pressed, up_pressed, down_pressed, right_pressed, center_pressed;

#endif