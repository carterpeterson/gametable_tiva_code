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
	Pixel *temp;
	int wait, count;
	Pixel p;
	
	
	p.red = 0;
	p.green = 0;
	p.blue = 0;
	
	init_input();
	init_led_display();
	init_capsense();
	count = 0;
	
	printf(":)");
	
	while(1) {
		// Do nothing, wait for frame buffer over UART
	}
	
	/*while(1) {
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
	}*/
	
}
