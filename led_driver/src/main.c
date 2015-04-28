// C
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"
#include "system_TM4C123.h"

// Project
#include "app_config.h"
#include "input.h"
#include "capsense.h"
#include "led_display.h"
#include "bus_lock.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"

int main(void)
{	
	#ifndef LED_BOARD
		//int i;
		// Wait for LED board to be up and running
		//for(i = 0; i < 160000000; i++) {}
	#endif
	
	init_bus_lock();
	init_input();
	init_led_display();
	init_capsense();
	
	poll_capsense();
	give_lock();
	
	while(1) {
		// Do nothing, wait for frame buffer over UART
	}	
}
