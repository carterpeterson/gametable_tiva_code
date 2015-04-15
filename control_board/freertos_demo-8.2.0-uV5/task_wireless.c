#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "TM4C123.h"

#include "task_console.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "../include/uart.h"

//*****************************************************************************
//
// The stack size for the Consoletask.
//
//*****************************************************************************
#define WIRELESS_TASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define WIRELESS_ITEM_SIZE           sizeof(uint8_t)
#define WIRELESS_QUEUE_SIZE          5


//*****************************************************************************
//
// The queue that holds messages sent to the LED task.
//
//*****************************************************************************
xQueueHandle g_pWirless_Queue;

extern xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
//
//*****************************************************************************
static void
WirelessTask(void *pvParameters)
{
    portTickType ui32WakeTime;
    char msg[40];
    uint32_t count = 0;

    while(1)
    {
      //
      // Get the current tick count.
      //
      ui32WakeTime = xTaskGetTickCount();
      
      // Format the string to print
      sprintf(msg,"WirlessCount %i\n\r",count);
      
      // Claim ownership of the UART
      xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
      
      // Send String
      uartTxPoll(UART0_BASE,msg);
      
      // Relinquish ownership of the UARTS
      xSemaphoreGive(g_pUARTSemaphore);
      
      // Wait 1 second before waking up
      vTaskDelayUntil(&ui32WakeTime, 100 / portTICK_RATE_MS);
      
      count++;
    }
    
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
WirelessTaskInit(void)
{
    
    BaseType_t return_val;
  
    //
    // Create the LED task.
    //
    return_val = xTaskCreate(
                              WirelessTask,                            // Function Pointer
                              (signed portCHAR *)"Wireless",           // Task Name
                              WIRELESS_TASKSTACKSIZE,                  // Stack Depth in Words
                              NULL,                                   // Parameters to Function
                              tskIDLE_PRIORITY + PRIORITY_LED_TASK,   // Task Priority
                              NULL);                                  // Task Handle

    if(return_val != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
