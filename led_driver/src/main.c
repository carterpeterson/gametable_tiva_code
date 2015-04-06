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
#include "capsense.h"

int main(void)
{	
	Pixel *temp;
	int wait, count;
	Pixel p;
	
	
	p.red = 0;
	p.green = 0;
	p.blue = 0;
	
	gpio_port_enable(PORT_B_CGC);
	gpio_digital_enable(PORT_B, (PIN_0));
	gpio_pin_direction(PORT_B, DIRECTION_OUTPUT, PIN_0);
	
	// Change the DIR on the 3v->5v pixel signal buffer to output
	gpio_port_enable(PORT_D_CGC);
	gpio_digital_enable(PORT_D, (PIN_2));
	gpio_pin_direction(PORT_D, DIRECTION_OUTPUT, PIN_2);
	PORT_D->DATA = 0x04;
	
	// Enable the UART0 to recieve frame buffer / debug output enable
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_0 | PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, (PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_INPUT, PIN_0);
	gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
	gpio_config_port_ctl(PORT_A, (PIN_0 | PIN_1), 1);
	
	init_input();
	init_screen_buffers();
	//init_capsense();
	count = 0;
	
	printf(":)");
	
	while(1) {
		for(wait = 0; wait < 1000000; wait++) {
			// wait
		}
		
		count += 10;
		count = count % 755;
		p.red = (count < 256) ? count : 0;
		p.green = (count > 255 && count < 512) ? count % 255 : 0;
		p.blue = (count > 512) ? count % 255 : 0;
		
		write_buffer[0] = p; 
		write_buffer[1] = p;
		write_buffer[2] = p;
		write_buffer[3] = p;
		write_buffer[4] = p;
		write_buffer[5] = p;
		write_buffer[6] = p;
		write_buffer[7] = p;
		
		temp = write_buffer;
		write_buffer = read_buffer;
		read_buffer = temp;
		
		convert_buffer();
		//switch_buffers();
	}
	
}
