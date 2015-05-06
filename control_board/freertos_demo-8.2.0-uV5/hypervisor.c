#include "hypervisor.h"

bool viewing_games = false;
uint8_t current_game_selected = 0;
uint8_t current_animation_selected = 4;
TaskHandle_t hypervisor_task, currently_running_task;

void task_hypervisor(void *pvParameters)
{
	BaseType_t return_val;
	TickType_t startup_time;
	
	startup_time = xTaskGetTickCount();
	
	vTaskDelayUntil(&startup_time, 1000);	// Let all the drivers settle down and whatnot
	
	//lcd_update();
	
	return_val = xTaskCreate(
                              GAMETABLE_ANIMATIONS[current_animation_selected].task_function,                     // Function Pointer
                        (signed portCHAR *)GAMETABLE_ANIMATIONS[current_animation_selected].name,           // Task Name
                              TASK_STACK_SIZE,                  									// Stack Depth in Words
                              NULL,                                   								// Parameters to Function (games and animations have none)
                              tskIDLE_PRIORITY + PRIORITY_ANIMATION,  								// Task Priority
                              &currently_running_task);                                  			// Task Handle

	while(1) {
		vTaskSuspend(NULL);
	
		if(center_pressed) {
			// change task
			vTaskDelete(currently_running_task);
			
			if(viewing_games) {
				xTaskCreate(
						GAMETABLE_GAMES[current_game_selected].task_function,                            	// Function Pointer
                        (signed portCHAR *)GAMETABLE_GAMES[current_game_selected].name,           			// Task Name
                        TASK_STACK_SIZE,                  													// Stack Depth in Words
                        NULL,                                   											// Parameters to Function (games and animations have none)
                        tskIDLE_PRIORITY + PRIORITY_GAME,  													// Task Priority
                        &currently_running_task);       
			} else {
				xTaskCreate(
						GAMETABLE_ANIMATIONS[current_animation_selected].task_function,                     // Function Pointer
                        (signed portCHAR *)GAMETABLE_ANIMATIONS[current_animation_selected].name,           // Task Name
                        TASK_STACK_SIZE,                  													// Stack Depth in Words
                        NULL,                                   											// Parameters to Function (games and animations have none)
                        tskIDLE_PRIORITY + PRIORITY_ANIMATION,  											// Task Priority
                        &currently_running_task);
			}
		} else if(left_pressed) {
			viewing_games = !viewing_games;
		} else if(up_pressed) {
			if(viewing_games) {
				if(current_game_selected == 0)
					current_game_selected = (NUM_GAMETABLE_GAMES - 1);
				else
					current_game_selected--;
			} else {
				if(current_animation_selected == 0)
					current_animation_selected = (NUM_GAMETABLE_ANIMATIONS - 1);
				else
					current_animation_selected--;
			}
		} else if(right_pressed) {
			viewing_games = !viewing_games;
		} else if(down_pressed) {
			if(viewing_games) {
				current_game_selected++;
				current_game_selected =	(current_game_selected % NUM_GAMETABLE_GAMES);
			} else {
				current_animation_selected++;
				current_animation_selected = (current_animation_selected % NUM_GAMETABLE_ANIMATIONS);
			}
		}
		
		// Clear all the buttons just in case
		center_pressed = false;
		left_pressed = false;
		up_pressed = false;
		right_pressed = false;
		down_pressed = false;
		
		//lcd_update();
	}
}

bool init_hypervisor_task(void)
{
	BaseType_t return_val;
  
    //
    // Create the LED task.
    //
    return_val = xTaskCreate(
                              task_hypervisor,                            	// Function Pointer
                              (signed portCHAR *)"hypervisor",           	// Task Name
                              TASK_STACK_SIZE,                  		// Stack Depth in Words
                              NULL,                                   	// Parameters to Function (games and animations have none)
                              tskIDLE_PRIORITY + PRIORITY_HYPERVISOR,  	// Task Priority
                              &hypervisor_task);    
}