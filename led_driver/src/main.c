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
#include "../library/i2c.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"
#include "input.h"

int wait;
I2C_request write_req, read_req;

void init_capsense(void)
{	
	//init_capsense_gpio();
	gpio_alternate_function_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_config_port_ctl(PORT_A, (PIN_6 | PIN_7), 3);
	gpio_open_drain_enable(PORT_A, PIN_7);
	//init_capsense_i2c();
	i2c_enable(I2C1_CGC);
	i2c_init_master(I2C1);
	i2c_master_interrupt_enable(I2C1, 1);
	
	
	write_req.device_addr = 0x20;
	write_req.read_req = false;
	write_req.data = 0xAE;
	write_req.size = 1;
	write_req.next_req = &read_req;
	
	read_req.device_addr = 0x20;
	read_req.read_req = true;
	read_req.size = 1;
	read_req.next_req = 0;
	read_req.data = 0;
	
	i2c_handle_request(&i2c1, &write_req);
		
	while(1) {
		if(i2c1.update_pending) {
			printf("h\r\n");
			i2c_handle_response(&i2c1);
		} else if (i2c1.busy == false) {
			printf("!\r\n");
			wait = 0;
			while(wait < 3000000)
				wait++;
				
			read_req.data = 0;	
			i2c_handle_request(&i2c1, &write_req);
		}
	}
}

int main(void)
{	
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_0 | PIN_1 | PIN_2 | PIN_6 | PIN_7));
	gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, (PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_INPUT, PIN_0);
	gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
	gpio_config_port_ctl(PORT_A, (PIN_0 | PIN_1), 1);
	
	init_input();
	init_screen_buffers();
	init_capsense();
	
	while(1) {
		//switch_buffers();
	}
	
}
