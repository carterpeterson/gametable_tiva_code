//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "../include/gpio.h"
#include "../include/uart.h"
#include "task_console.h"
#include "task_wireless.h"


#define PA0 (1 << 0)
#define PA1 (1 << 1)


//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
// Configure PA0 and PA1 to be UART pins
//*****************************************************************************
void uart0_config_gpio(void)
{
   gpio_enable_port(GPIOA_BASE);
   gpio_config_digital_enable( GPIOA_BASE, PA0 | PA1);
   gpio_config_alternate_function( GPIOA_BASE, PA0 | PA1);
   gpio_config_port_control( GPIOA_BASE, GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
}



//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  //
  // Create a mutex to guard the UART.
  //
  g_pUARTSemaphore = xSemaphoreCreateMutex();
  
  uart0_config_gpio();
  if(uart_init_115K(UART0_BASE) == false)
  {
     while(1)
     {
        // Halt the program
     }
  }
  uartTxPoll(UART0_BASE,"\n\r");
  uartTxPoll(UART0_BASE,"*********************\n\r");
  uartTxPoll(UART0_BASE,"ECE453 FreeRTOS Demo Test 2\n\r");
  uartTxPoll(UART0_BASE,"*********************\n\r");
    

 // Start the console task
  if( ConsoleTaskInit()!= 0)
  {
    while(1){};
  }

 // Start the wireless task
  if( WirelessTaskInit()!= 0)
  {
    while(1){};
  }
  
  //
  // Start the scheduler.  This should not return.
  //
  vTaskStartScheduler();
  
  while(1)
  {
     // Infinite Loop
  }
}
