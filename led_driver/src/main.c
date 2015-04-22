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
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"
#include "input.h"
#include "capsense.h"
#include "led_display.h"

int main(void)
{	
	init_input();
	init_led_display();
	//init_capsense();
	
	UART0->DR = '!';
	
	while(1) {
		// Do nothing, wait for frame buffer over UART
	}	
}
