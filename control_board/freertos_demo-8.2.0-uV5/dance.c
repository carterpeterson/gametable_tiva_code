#include "dance.h"

void task_dance_animation(void *pvParameters)
{
	while(1) {
		vTaskDelay(TICK_DELAY_30_FPS);
	}
}