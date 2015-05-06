#ifndef __DRAWER_H__
#define	__DRAWER_H__

// General C includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// FreeRTOS includes
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Project specific includes
#include "tasks.h"
#include "led_display.h"
#include "touch_input.h"
#include "mario_world.h"

#define SCROLL_SPEED_MS			150

#define COLOR_CHANNELS_RGBA		4
#define MARIO_WORLD_WIDTH		82

#endif