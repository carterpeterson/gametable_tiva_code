#ifndef __CAPSENSE_H__
#define __CAPSENSE_H__

// C
#include <stdint.h>
#include <stdbool.h>

// MCU

// Project
#include "input.h"
#include "convert_buffer.h"
#include "app_config.h"
#include "../library/gpio.h"
#include "../library/i2c.h"

void init_capsense(void);

#endif
