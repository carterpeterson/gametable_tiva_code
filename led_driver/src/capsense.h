#ifndef __CAPSENSE_H__
#define __CAPSENSE_H__

// C
#include <stdint.h>
#include <stdbool.h>

// MCU

// Project
#include "input.h"
#include "interrupt_routines.h"
#include "app_config.h"
#include "../library/gpio.h"
#include "../library/i2c.h"

#define PIXELS_PER_TOUCH_SECTION 			32
#define CAPSENSE_CONTROLLERS_PER_SECTION 	16
#define CAPSENSE_PROX_REG_ADDR				0xAE

#define BIT_0 0x01
#define BIT_1 0x02

#define TOUCH_BUFFER_SIZE				4
#define TOUCH_BUFFER_MINUS_ONE 			15

#ifdef USING_SIMULATOR
	#define CAPSENSE_OUTPUT_DMA_CHANNEL 9
#else
	#define CAPSENSE_OUTPUT_DMA_CHANNEL 1
#endif

void init_capsense(void);
void poll_capsense(void);
extern bool poll_capsense_req, pushing_uart;

#endif
