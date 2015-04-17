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

#include "led_display.h"


//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

#define BLOOM_RADIUS 3
#define FULL_RADIUS_STEP 100

static uint16_t current_color_base = 0;
static float MAX_RADIUS;

Pixel get_bloom_pixel(int i, int j)
{
  Pixel p;
  uint16_t current_color = current_color_base;

  current_color += (uint16_t) ((hypot((float) i, (float) j) / MAX_RADIUS) * FULL_RADIUS_STEP);
  current_color = current_color % 768;
  
  if(current_color < 256) {
    p.red = (255 - current_color);
    p.green = current_color;
    p.blue = 0;
  } else if(current_color < 512) {
    p.red = 0;
    p.green = (511 - current_color);
    p.blue = (current_color - 256);
  } else {
    p.red = (current_color - 512);
    p.green = 0;
    p.blue = 767 - current_color ;
  }

  return p;
}

void bloom_animation(void)
{
  int i, j;
  Pixel white_pixel;
  MAX_RADIUS = hypot(BLOOM_RADIUS, BLOOM_RADIUS);
  current_color_base += 1;
  white_pixel.red = 255;
  white_pixel.green = 255;
  white_pixel.blue = 255;

  if(current_color_base > (256 * 3))
    current_color_base = 0;

  for(i = 0; i < 32; i++) {
    int temp = i;
    
    for(j = 0; j < 8; j++) {
      
      Pixel p;
      if((i % 6) < 3 && (j % 6) < 3) {
	p = get_bloom_pixel((i % 3), (j % 3));
      } else if ((i % 6) >= 3 && (j % 6) < 3) {
	p = get_bloom_pixel(3 - (i % 3), (j % 3));	
      } else if ((i % 6) < 3 && (j % 6) >= 3) {
	p = get_bloom_pixel((i % 3), 3 - (j % 3));	
      } else {
	p = get_bloom_pixel(3 - (i % 3), 3 - (j % 3));	
      }

	frame_buffer[(j * 32) + i] = p;
    }
    i = temp;
  }

  render();
}


//*****************************************************************************
//*****************************************************************************
int main(void)
{
	Pixel p;
	int i, j;
	init_led_display();

	//
	// Create a mutex to guard the UART.
	//
	//g_pUARTSemaphore = xSemaphoreCreateMutex();

	//
	// Start the scheduler.  This should not return.
	//
	//vTaskStartScheduler();

	while(1)
	{
	 bloom_animation();
	 for(i = 0; i < 30000; i++) {
		// Wait
	 }
	}
}

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