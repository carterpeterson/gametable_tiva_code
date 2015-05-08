// General C includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Project specific includes
#include "tasks.h"
#include "lookup_table.h"


void draw_eyes(uint8_t eye_num)
{
	uint8_t* current_eyes, i, j;
	Pixel p;
	
	switch(eye_num) {
	case 1:
		current_eyes = (uint8_t *) &eye1[0][0][0];
		break;
	case 2:
		current_eyes = (uint8_t *) &eye2[0][0][0];
		break;
	case 3:
		current_eyes = (uint8_t *) &eye3[0][0][0];
		break;
	case 4:
		p.blue = 255;
		for(i = 0; i < 32; i++) {
			for(j = 0; j < 8; j++) {
				set_pixel(i, j, p);
			}
		}
	
		render();
		return;
		break;
	case 5:
		current_eyes = (uint8_t *) &eye1[0][0][0];
		break;
	case 6:
		current_eyes = (uint8_t *) &look2[0][0][0];
		break;
	case 7:
		current_eyes = (uint8_t *) &look3[0][0][0];
		break;
	case 8:
		current_eyes = (uint8_t *) &look4[0][0][0];
		break;
	default:
		break;
	}
	
	for(i = 0; i < 32; i++) {
		for(j = 0; j < 8; j++) {
			p.red = current_eyes[(i * 8 * 3) + (j * 3)];
			p.green = current_eyes[(i * 8 * 3) + (j * 3) + 1];
			p.blue = current_eyes[(i * 8 * 3) + (j * 3) + 2];
			set_pixel(i, j, p);
		}
	}
	
	render();
}

void blink_eyes(void){
	draw_eyes(1);
	vTaskDelay(133);
	
	draw_eyes(2);
	vTaskDelay(133);
	
	draw_eyes(3);
	vTaskDelay(133);
	
	draw_eyes(4);
	vTaskDelay(133);
	
	draw_eyes(3);
	vTaskDelay(133);
	
	draw_eyes(2);
	vTaskDelay(133);
	
	draw_eyes(5);
	vTaskDelay(133);
	
	draw_eyes(6);
	vTaskDelay(133);
	
	draw_eyes(7);
	vTaskDelay(133);
	
	draw_eyes(6);
	vTaskDelay(133);
	
	draw_eyes(5);
	vTaskDelay(133);
	
	draw_eyes(8);
	vTaskDelay(133);
}

void task_eyes_animation(void *pvParameters)
{
  
  while(1) {   
    blink_eyes();
	vTaskDelay(133);
  }
}

