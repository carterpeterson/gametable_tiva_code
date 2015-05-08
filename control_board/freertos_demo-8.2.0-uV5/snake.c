#include "snake.h"

//game matrix
int snakeMat[GRIDX][GRIDY];

//head location
uint32_t headX;
uint32_t headY;

//fruit location
uint32_t fruitX;
uint32_t fruitY;

//tail location
uint32_t tailX;
uint32_t tailY;

uint8_t direction;
uint16_t snakeLength;
int tailDir;
int next;

int bufferUpdates;

//extern void DisableInterrupts();
//extern void EnableInterrupts();

bool gameOver;
//volatile bool AlertSnake;

Pixel green_pixel;
Pixel red_pixel;
Pixel blank_pixel;
Pixel blue_pixel;
int curr;

void updateFrameBuffer(void);

void initGame(void){

  int i, j;
	//initialize game variables
	headX = 2;
	headY = 2;
	
	tailX = 1;
	tailY = 2;
	fruitX = 5;
	fruitY = 5;
	snakeLength = 2;
	bufferUpdates = 0;
 
   for(i = 0; i < GRIDX; i++){
     for(j = 0; j < GRIDY; j++){
       snakeMat[i][j] = BLANKSPACE;
     }
   }
 
	snakeMat[headX][headY] = snakeLength;
	snakeMat[tailX][tailY] = UP;
	snakeMat[fruitX][fruitY] = FRUIT;
	direction = UP;
	gameOver = false;
	bufferUpdates = 0;
	
    red_pixel.red = 255;
    red_pixel.green = 0;
    red_pixel.blue = 0;
    
    green_pixel.red = 0;
    green_pixel.green = 255;
    green_pixel.blue = 0;
	
	blank_pixel.red = 0;
    blank_pixel.green = 0;
    blank_pixel.blue = 0;
    
    blue_pixel.red = 0;
    blue_pixel.green = 0;
    blue_pixel.blue = 255;
}

void endGame(){
	int x, y;
  
  while(1){
  
   for(x = 0; x < 32; x++){
     for(y = 0; y < 8; y++){
        if((x == 2 || x == 3) && (y == 3 || y == 4)){
            frame_buffer[x + y*32] = green_pixel;
        }
        else if(((x == 22) && (y == 3)) ||
        ((x == 22) && (y == 4)) ||
        ((x == 21) && (y == 2)) ||
        ((x == 21) && (y == 5)) ||
        ((x == 24) && (y == 2)) ||
        ((x == 20) && (y == 1)) ||
        ((x == 20) && (y == 6)) ||
        ((x == 24) && (y == 5))){
               frame_buffer[x + y*32] = blue_pixel;
        }
        else{
            frame_buffer[x + y*32] = red_pixel;
        }
     }
   }

    for(x = 2; x <= 3; x++){
      for(y = 3; y <= 4; y++){
        if(is_pixel_touched(x,y)){
          return;
        }
      }
    }
    render();
	vTaskDelay(TICK_DELAY_30_FPS);
    //updateFrameBuffer();
  }
}

int moveSnake(void){
	int dir, r;
	dir = direction;
	
	//curr = 0;

	//set previous head spot with direction the head is about to travel
	snakeMat[headX][headY] = dir;
	//get the direction the tail needs to travel
	tailDir = snakeMat[tailX][tailY];
	
	//move head
	switch(dir){
		case(UP):
			headX = (headX + 1) % GRIDX;
			break;
		case(RIGHT):
			headY = (headY + 1) % GRIDY;
			break;
		case(DOWN):
			headX = (headX + (GRIDX-1)) % GRIDX;
			break;
		case(LEFT):
			headY = (headY + (GRIDY-1)) % GRIDY;
			break;
	}
	//look at the spot we are about to move the head
	next = snakeMat[headX][headY];
	
	//set that to the current length of the snake
	snakeMat[headX][headY] = snakeLength;
	
	//if the next spot is a fruit
	if(next == FRUIT){
			//fruitX = 0;
			//fruitY = 12;
			//eat that fruit! (don't move tail)
			//determine random location of next fruit
			
			do{
			    //r = randVals[curr];
				r = rand() % (GRIDX*GRIDY);
				fruitX = r/GRIDY;
				fruitY = r % GRIDY;
				curr++;
			}while(snakeMat[fruitX][fruitY] != 0);
			snakeMat[fruitX][fruitY] = FRUIT;
			
			//increase length of snake
			snakeLength++;
	}
	else if(next != 0){
		//if the next spot is a snake piece
		//uartTxPoll(UART0,"Game Over!");
		//set game over and call end game loop
		gameOver = true;
        return 1;
	}
	else{
		//otherwise move the tail
		snakeMat[tailX][tailY] = 0;
		switch(tailDir){
			case(UP):
				tailX = (tailX + 1) % GRIDX;
				break;
			case(RIGHT):
				tailY = (tailY + 1) % GRIDY;
				break;
			case(DOWN):
				tailX = (tailX + (GRIDX-1)) % GRIDX;
				break;
			case(LEFT):
				tailY = (tailY + (GRIDY-1)) % GRIDY;
				break;
		}

	}
   return 0;
}

void examineButtons(void)
{  
	bool go_left, go_right, go_up, go_down;
	uint8_t left, right, up, down;
	go_left = true;
	go_right = true;
	go_up = true;
	go_down = true;
	left = 0;
	right = 0;
	up = 0;
	down = 0;

	if(is_pixel_touched(0,0))
		down++;
	if(is_pixel_touched(0,1))
		down++;
	if(is_pixel_touched(1,0))
		down++;
	if(is_pixel_touched(1,1))
		down++;

	if(is_pixel_touched(0,2))
		right++;
	if(is_pixel_touched(1,2))
		right++;
	if(is_pixel_touched(0,3))
		right++;
	if(is_pixel_touched(1,3))
		right++;
		
	if(is_pixel_touched(0,4))
		left++;
	if(is_pixel_touched(1,4))
		left++;
	if(is_pixel_touched(0,5))
		left++;
	if(is_pixel_touched(1,5))
		left++;
		
	if(is_pixel_touched(0,6))
		up++;
	if(is_pixel_touched(1,6))
		up++;
	if(is_pixel_touched(0,7))
		up++;
	if(is_pixel_touched(1,7))
		up++;

	if(direction == RIGHT || left < up || left < down || left < right)
		go_left = false;
	if(direction == LEFT || right < up || right < down || right < left)
		go_right = false;		
	if(direction == UP || down < up || down < right || down < left)
		go_down = false;		
	if(direction == DOWN || up < down || up < right || up < left)
		go_up = false;		
	
	if(go_left)
		direction = LEFT;
	else if(go_down)
		direction = DOWN;
	else if(go_right)
		direction = RIGHT;
	else if(go_up)
		direction = UP;
	
	
	return;
}

void updateFrameBuffer(void){

    int x,y;
    Pixel orange_pixel, yellow_pixel, purp_pixel;
    
    if(bufferUpdates <= 1){
    
        
        orange_pixel.red = 225;
        orange_pixel.green = 153;
        orange_pixel.blue = 0;
        
        yellow_pixel.red = 255;
        yellow_pixel.green = 255;
        yellow_pixel.blue = 0;
        
        purp_pixel.red = 204;
        purp_pixel.green = 0;
        purp_pixel.blue = 204;
    
        frame_buffer[0] = orange_pixel;
        frame_buffer[1] = orange_pixel;
        frame_buffer[32] = orange_pixel;
        frame_buffer[33] = orange_pixel;
        
        frame_buffer[64] = blue_pixel;
        frame_buffer[65] = blue_pixel;
        frame_buffer[96] = blue_pixel;
        frame_buffer[97] = blue_pixel;
        
        frame_buffer[128] = yellow_pixel;
        frame_buffer[129] = yellow_pixel;
        frame_buffer[160] = yellow_pixel;
        frame_buffer[161] = yellow_pixel;
        
        frame_buffer[192] = purp_pixel;
        frame_buffer[193] = purp_pixel;
        frame_buffer[224] = purp_pixel;
        frame_buffer[225] = purp_pixel;
        
        bufferUpdates++;
    }
    
    for(x = 0; x < GRIDX; x++){
        for(y = 0; y < GRIDY; y++){
            if(snakeMat[x][y] > 0){
                frame_buffer[PIXELS_WIDTH*x + ((PIXELS_WIDTH-1)-y)] = red_pixel;
            }
            else if(snakeMat[x][y] < 0){
                frame_buffer[PIXELS_WIDTH*x + ((PIXELS_WIDTH-1)-y)] = green_pixel;
            }
            else{
                frame_buffer[PIXELS_WIDTH*x + ((PIXELS_WIDTH-1)-y)] = blank_pixel;
            }
        }
    }
    render();
}

void task_snake_game(void *pvParameters)
{
    int score;
	TickType_t curr_time, prev_time;

    curr_time = xTaskGetTickCount();
    prev_time = xTaskGetTickCount();    

    
    initGame();
    while(1) {
        curr_time = xTaskGetTickCount();
    
        if((prev_time + SNAKE_SPEED_TICKS) < curr_time){
		updateFrameBuffer();
        examineButtons();
            if((moveSnake())){
              endGame();
              initGame();
              
              curr_time = xTaskGetTickCount();
            }
            prev_time = curr_time;
        }
		vTaskDelay(TICK_DELAY_30_FPS);
    }
}
	
