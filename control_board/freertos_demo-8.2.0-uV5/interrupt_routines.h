#ifndef __INTERRUPT_ROUTINES_H__
#define __INTERRUPT_ROUTINES_H__

// C
#include <stdint.h>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Project
#include "app_config.h"
#include "led_display.h"
#include "touch_input.h"
#include "lcd_menu.h"
#include "../drivers/dma.h"
#include "../drivers/i2c.h"
#include "../drivers/gpio.h"
#include "button_input.h"

#endif
