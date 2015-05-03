#ifndef __SNAKE_H__
#define __SNAKE_H__

// General C includes
#include <stdlib.h>
#include <math.h>

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

#define UP 1
#define DOWN 3
#define LEFT 4
#define RIGHT 2
#define FRUIT -1
#define BLANKSPACE 0

#define GRIDX 8
#define GRIDY 30

#define SNAKE_SPEED_TICKS 100

#endif
