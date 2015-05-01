#include "hypervisor.h"

void task_hypervisor(void *pvParameters)
{
	BaseType_t return_val;
	TickType_t startup_time;
	TaskHandle_t created_task;
	
	startup_time = xTaskGetTickCount();
	
    return_val = xTaskCreate(
                              task_snake_game,                            	// Function Pointer
                              (signed portCHAR *)"snake",           		// Task Name
                              TASK_STACK_SIZE,                  		// Stack Depth in Words
                              NULL,                                   	// Parameters to Function (games and animations have none)
                              tskIDLE_PRIORITY + PRIORITY_ANIMATION,  	// Task Priority
                              &created_task);                                  	// Task Handle
	
	
	vTaskDelayUntil(&startup_time, 1000);

	/*xTaskCreate(
				  task_pacman_animation,                   	// Function Pointer
				  (signed portCHAR *)"pacman",           	// Task Name
				  TASK_STACK_SIZE,                  		// Stack Depth in Words
				  NULL,                                   	// Parameters to Function (games and animations have none)
				  tskIDLE_PRIORITY + PRIORITY_ANIMATION,  	// Task Priority
				  &created_task);*/   

	while(1) {
		vTaskDelayUntil(&startup_time, 1);
	
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
                              NULL);    
}