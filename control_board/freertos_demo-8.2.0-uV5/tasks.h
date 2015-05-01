#ifndef __TASKS_H__
#define __TASKS_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "led_display.h"
#include "touch_input.h"

#define TASK_STACK_SIZE        	1024         // Stack size in words

#define PRIORITY_ANIMATION		2
#define PRIORITY_GAME			2

#define TICK_DELAY_30_FPS		33

//void task_pacman_animation(void *pvParameters);
void task_snake_game(void *pvParameters);

#endif