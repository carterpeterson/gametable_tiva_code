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

#define PIXELS_PER_TOUCH_SECTION 	32


extern uint32_t *touch_read_buffer;

void init_capsense(void);

#endif
