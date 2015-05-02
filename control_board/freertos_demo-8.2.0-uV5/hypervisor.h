#ifndef __HYPERVISOR_H__
#define __HYPERVISOR_H__

#include <stdbool.h>
#include <stdint.h>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Gametable includes (sweg)
#include "tasks.h"
#include "lcd_menu.h"
#include "button_input.h"

#define PRIORITY_HYPERVISOR 5

bool init_hypervisor_task(void);

extern TaskHandle_t hypervisor_task;
extern bool viewing_games;
extern uint8_t current_game_selected, current_animation_selected;

#endif