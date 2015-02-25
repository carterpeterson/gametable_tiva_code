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
#include "../library/led_display.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"
#include "input.h"

int main(void)
{	
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_0 | PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, (PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_INPUT, PIN_0);
	gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
	PORT_A->PCTL = 0x0011;
	
	init_input();
	init_screen_buffers();
	
	while(1) {
		//switch_buffers();
	}
	
}
