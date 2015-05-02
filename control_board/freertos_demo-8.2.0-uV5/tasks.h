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

typedef void (*FreeRTOSTaskFunc)(void *pvParameters);

typedef struct {
	FreeRTOSTaskFunc task_function;
	char* name;
} GameTableTask;

// Gametable animation declarations
void task_snake_game(void *pvParameters);

// Gametable game declarations
void task_bloom(void *pvParameters);


#define NUM_GAMETABLE_GAMES			1
#define NUM_GAMETABLE_ANIMATIONS	1

const static GameTableTask GAMETABLE_GAMES[] = {
	{task_snake_game , "Snake"}
};

const static GameTableTask GAMETABLE_ANIMATIONS[] = {
	{task_bloom , "Bloom"}
};


#endif