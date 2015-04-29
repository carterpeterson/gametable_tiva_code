#ifndef __BLOOM_H__
#define __BLOOM_H__

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "tasks.h"
#include "led_display.h"
#include "touch_input.h"


#define BLOOM_RADIUS 3
#define FULL_RADIUS_STEP 100

void task_bloom(void *pvParameters);

#endif