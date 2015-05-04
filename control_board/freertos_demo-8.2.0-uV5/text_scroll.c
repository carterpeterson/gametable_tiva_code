// General C includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Project specific includes
#include "tasks.h"
#include "lookup_table.h"
#include "lookup_letter.h"

int scroll_j = 0;
bool dir = true;
bool isTouched = false;
int debnc = 0;

//draws image with width wi and height hj from top left corner i,j
void draw(short int i, short int j, uint8_t wi, uint8_t hj, bool wrap, const short int image[wi][hj][3], int start_i){
  int jstart;
  int k, z;
  Pixel p;
  //store starting row
  jstart = j;
  
  //check if top left corner falls within table boundaries
  if((i > 31) || (j > 7)){
    return;
  }
  
  for(k = start_i; k < wi + start_i; k++){
    for(z = 0; z < hj; z++){
      //get color of each pixel
      if(image[k][z][0] != -1){
        p.red = image[k][z][0];
      }else{
        p.red = 255;
      }
      if(image[k][z][1] != -1){
        p.green = image[k][z][1];
      }else{
        p.green = 255;
      }
      if(image[k][z][2] != -1){
        p.blue = image[k][z][2];
      }else{
        p.blue = 255;
      }
      
      //if wrap is on, adjust i,j
      if(wrap){
        if(i < 0){
          i = 31 + i;
        }
        if(i > 31){
          i = i - 30;
        }
        if(j < 0){
          j = j + 7;
        }
        if(j > 7){
          j = 6 - j;
        }
      }
      
      //if pixel is in visible region, set pixel
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

}

void scroll_text(const short int font[768][8][3], const short int char_width[96], char text[]){
  int i = 0;
  int j = 31;
  int k = 0;
  int val;
  int size = 0;
  char *ptr; //used to traverse string

  ptr = text;
  
  //find if board is being touched and debounce
  for(i = 0; i < 32; i++){
    for(k = 0; k < 8; k++){
      if(is_pixel_touched(i, k)){
        isTouched = true;
        k = 8; i = 32;
      }else{
        isTouched = false;
      }
    }
  }
  if(isTouched){
    debnc++;
    if(debnc == 1){
      dir = !dir;
    }
  }else{
    debnc = 0;
  
  }

  //draw character
  for(i = 0; i < strlen(text); i++){
    //get decimal value of char
    val = *ptr;

    draw(j + scroll_j, 0, char_width[val-32], 8, 0, Small_Fonts8x8, (val-32)*8);

    
    ptr++;
    j+=char_width[val-32];
  
  }
  
  //if scroll left
  if(dir){
    scroll_j--;
    if(j + scroll_j == 0){
      
      scroll_j = 0;
  
    }
  //if scroll right
  }else{
    scroll_j++;
    if(j - scroll_j == 131){
      scroll_j = -j;
    }
  }
  render();
}


void demo_task(void)
{
  int i = 0;
  while(1) {
    
    usleep(99993);
    //draw a background
    draw(0,0, 32, 8, 0, default_format, 0);
    //Scroll text
    scroll_text(Small_Fonts8x8, Small_Fonts8x8_width, "I am Tobias. Fear me.");
  }
}

