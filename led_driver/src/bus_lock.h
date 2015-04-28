#ifndef __H_BUS_LOCK__
#define __H_BUS_LOCK__

// C
#include <stdint.h>
#include <stdbool.h>

// MCU

// Project
#include "input.h"
#include "interrupt_routines.h"
#include "app_config.h"
#include "../library/gpio.h"
#include "../library/uart.h"
#include "../library/i2c.h"

#ifdef LED_BOARD
	extern bool bus_lock_top, bus_lock_bottom;
#else
	extern bool bus_lock;
#endif

void init_bus_lock(void);
void give_lock(void);
bool get_lock(uint8_t lock_code);

#endif