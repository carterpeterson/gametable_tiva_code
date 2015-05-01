// General C includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Project specific includes
#include "tasks.h"
#include "lookup_table.h"

static int iscroll = 0;
uint8_t stage = 0;
bool mouth = true;


//draws image with width wi and height hj from top left corner i,j
void draw(short int i, short int j, uint8_t wi, uint8_t hj, bool wrap, const short *image){
  
  int jstart;
  int k, z;
  Pixel p;
  //store starting row
  jstart = j;
  
  //check if top left corner falls within table boundaries
  if((i > 31) || (j > 7)){
    return;
  }
  
  for(k = 0; k < wi; k++){
    for(z = 0; z < hj; z++){
      //get color of each pixel
      if(image[k * (3 * hj) + z * 3 + 0] == -2){
        if(mouth){
          p.red = 255;
          
        }else{
          p.red = 255;

        }
      }
      else if(image[k * (3 * hj) + z * 3 + 0] != -1){
        p.red = image[k * (3 * hj) + z * 3 + 0];
      }else{
        p.red = 255;
      }
      if(image[k * (3 * hj) + z * 3 + 0] == -2){
        if(mouth){
          p.green = 255;

        }else{
          p.green = 255;

        }
      }else if(image[k * (3 * hj) + z * 3 + 0] != -1){
        p.green = image[k * (3 * hj) + z * 3 + 0];
      }else{
        p.green = 255;
      }
      if(image[k * (3 * hj) + z * 3 + 0] == -2){
        if(mouth){
          p.blue = 0;
 
        }else{
          p.blue = 255;
    
        }
      }else if(image[k * (3 * hj) + z * 3 + 0] != -1){
        p.blue = image[k * (3 * hj) + z * 3 + 0];
      }else{
        p.blue = 255;
      }
      
      //if pixel is in visible region set pixel
      //if wrap is on, adjust i,j
      if(wrap){
        if(i < 0){
          i = 31 + i;
        }
        if(i > 31){
          i = i - 31-1;
        }
        if(j < 0){
          j = j + 7;
        }
        if(j > 7){
          j = 7 - j-1;
        }
        //set_pixel(i, j, p);
      }
      if(i >= 0 && j >= 0){
        set_pixel(i, j, p);
      }
      //increment to next row
      j++;
      if(j > 7){
        j = jstart;
        break;
      }
    } 
    //increment to next column
    i++;
    if(i > 31 && !wrap){
      break;
    }
  }
  
  render();
  
}

//scrolls image to left or right
void scroll(int dir, int *start, uint8_t wi, uint8_t hj, const short *image){
  draw(*start += dir, 0, wi, hj, 0, image);
}

void task_pacman_animation(void *pvParameters)
{
	TickType_t startup_time;
	int *start;
	bool pac = true;
	*start = 32;
	
	while(1) {
		vTaskDelay(73);
		mouth = !mouth;
		draw(0,0, 32, 8, 0, &default_format[0][0][0]);
		if(stage == 0){

			scroll(-1, start, 37, 8, &pacman_retreat_right[0][0][0]);
			if(*start + 37 == 0){  
				stage = (stage + 1)%4;
			}
		}else if(stage == 1){
			scroll(1, start, 37, 8, &pacman_retreat_left[0][0][0]);
			if(*start == 32){
				stage = (stage + 1)%4;
			}
		}else if(stage == 2){
			scroll(-1, start, 37, 8, &pacman_offense[0][0][0]);
			if(*start + 37 == 0){
				stage = (stage + 1)%4;
				*start = -7;
			}
		}else{
			scroll(1, start, 7, 8, &pacman[0][0][0]);
			if(*start == 32){
				stage = (stage + 1)%4;
				vTaskDelay(73);
			}
		}
	}
}

