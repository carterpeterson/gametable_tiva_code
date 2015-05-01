#ifndef __PONG_H__
#define __PONG_H__

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "tasks.h"
#include "led_display.h"
#include "touch_input.h"

#define UP -1
#define HOLDY 0
#define DOWN 1

#define LEFT -1
#define HOLDX 0
#define RIGHT 1

#define BALL -1
#define BALLTAIL -2
#define LPAD 1
#define RPAD 2
#define BLANKSPACE 0

#define GRIDX 32
#define GRIDY 8

void task_pong_game(void *pvParameters);

#endif
