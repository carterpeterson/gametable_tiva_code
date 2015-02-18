#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "TM4C123.h"
#include "system_TM4C123.h"

#include "../library/led_display.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/dma.h"
#include "../library/uart.h"

void usleep(int microseconds)
{
		int i = 90000;
		while(i > 0)
			i--;
}

void do_led_stuff(void)
{	
	int i, stage;
	uint8_t red[3];
	uint8_t	green[3];
	uint8_t	blue[3];
	Pixel frame_buffer[5];
	i = 1;
	
	for(i = 0; i < 5; i++) {
		frame_buffer[i].green = 0;
		frame_buffer[i].blue = 0;
		frame_buffer[i].red = 0;
	}
		
	stage = 0;
	red[0] = 255;
	green[0] = 0;
	blue[0] = 0;
	
	red[1] = 0;
	green[1] = 0;
	blue[1] = 255;
	
	red[2] = 0;
	green[2] = 255;
	blue[2] = 0;
	
	while(1){		
		if(stage == 0) {
			red[0]--;
			green[0]++;
			
			red[1]++;
			blue[1]--;
			
			green[2]--;
			blue[2]++;
			
			if(green[0] == 255)
				stage = 1;
		} else if(stage == 1) {
			green[0]--;
			blue[0]++;
			
			green[1]++;
			red[1]--;
			
			blue[2]--;
			red[2]++;
			
			if(blue[0] == 255)
				stage = 2;
		} else {
			blue[0]--;
			red[0]++;
			
			green[1]--;
			blue[1]++;
			
			red[2]--;
			green[2]++;
			
			if(red[0] == 255)
				stage = 0;
		}
		
		frame_buffer[0].red = red[0];
		frame_buffer[0].blue = blue[0];
		frame_buffer[0].green = green[0];
		
		frame_buffer[1].red = red[0];
		frame_buffer[1].blue = blue[0];
		frame_buffer[1].green = green[0];
		
		/*frame_buffer[3].red = red[1];
		frame_buffer[3].blue = blue[1];
		frame_buffer[3].green = green[1];
		
		frame_buffer[2].red = red[2];
		frame_buffer[2].green = green[2];
		frame_buffer[2].blue = blue[2];
		
		frame_buffer[4].red = red[2];
		frame_buffer[4].green = green[2];
		frame_buffer[4].blue = blue[2];*/
		
		
		while((TIMER0->RIS & 0x1) != 0x1) {
			; // Do nothing
		}
		TIMER0->ICR = 0x01;
		pushBuffer(frame_buffer);
	}
}


int main(void)
{	
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_0 | PIN_1 | PIN_2));
	gpio_pin_direction(PORT_A, DIRECTION_OUTPUT, (PIN_0 | PIN_1 | PIN_2));
	gpio_alternate_function_enable(PORT_A, (PIN_0 | PIN_1));
	PORT_A->PCTL = 0x0011;
	
	//timer_init(TIMER0);
	//timer_periodic_enable(TIMER0);
	
	uart_enable(UART0_CGC);
	uart_configure(UART0, 500000);
	
	dma_enable();
	dma_test();
	
	while(1)
		; // Don't exit
	
	//do_led_stuff();
}
