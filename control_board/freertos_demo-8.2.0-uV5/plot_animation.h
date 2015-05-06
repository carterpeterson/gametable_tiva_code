#ifndef __PLOT_ANIMATION_H__
#define	__PLOT_ANIMATION_H__

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

#define ROTATION_SPEED 		3.5 // degrees per tick
#define SIZE_SCALE_X 		10
#define SIZE_SCALE_Y 		4
#define COLOR_SCALAR 		255
#define ZOOM_BASE 			0.8f
#define ZOOM_RANGE 			0.3f
#define ZOOM_SPEED 			0.6

#define COLOR_BASE 			0.75
#define COLOR_RANGE 		0.1
#define COLOR_SCROLL_SPEED 	0.3

#define WOBBLE_X_SPEED 		1.4
#define WOBBLE_Y_SPEED 		0
#define BASE_POSITION_X 	16
#define BASE_POSITION_Y 	4
#define WOBBLE_DISTANCE_X 	6
#define WOBBLE_DISTANCE_Y 	0.8

#define M_PI				3.141592653589793238

#endif