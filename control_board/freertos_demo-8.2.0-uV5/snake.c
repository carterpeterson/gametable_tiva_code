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
int curr;


void initGame(void){
	//initialize game variables
	headX = 2;
	headY = 2;
	
	tailX = 1;
	tailY = 2;
	fruitX = 5;
	fruitY = 5;
	snakeLength = 2;
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
}

void endGame(int score){
    gameOver = true;
}

void moveSnake(void){
	int dir, r;
	int randVals[] = {84, 27, 183, 101, 78, 200, 8, 93, 138, 55, 220, 7};
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
				//printf("%d, %d \n", fruitX, fruitY);
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
		endGame(snakeLength);
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
}

void examineButtons(void)
{  
	
	//if button has been pressed determine which has bee n pressed
	//and set Direction var to appropriate value

		if((is_pixel_touched(0,2) || is_pixel_touched(1,2) ||
		    is_pixel_touched(0,3) || is_pixel_touched(1,3)) && direction != RIGHT) {
			//uartTxPoll(UART0, "SW300 Button Pushed\n\r");
		    //printf("LEFT");
			direction = LEFT;
		}
		if((is_pixel_touched(0,6) || is_pixel_touched(1,6) ||
		    is_pixel_touched(0,7) || is_pixel_touched(1,7))&& direction != DOWN) {
			//uartTxPoll(UART0, "SW301 Button Pushed\n\r");
			//printf("up");
			direction = UP;
		}
		if((is_pixel_touched(0,4) || is_pixel_touched(1,4) ||
		    is_pixel_touched(0,5) || is_pixel_touched(1,5))&& direction != LEFT) {
			//uartTxPoll(UART0, "SW302 Button Pushed\n\r");
		    //printf("RIGHT");
			direction = RIGHT;			
		}
		if((is_pixel_touched(0,0) || is_pixel_touched(0,1) ||
		    is_pixel_touched(1,0) || is_pixel_touched(1,1))&& direction != UP) {
			//uartTxPoll(UART0, "SW303 Button Pushed\n\r");
			//printf("DOWN");
			direction = DOWN;
		}
		//AlertDebounce = false;
	
	return;
}

void updateFrameBuffer(void){

    int x,y;
	Pixel purp_pixel, blue_pixel, orange_pixel, yellow_pixel;
    
    if(bufferUpdates <= 1){
    
        blue_pixel.red = 0;
        blue_pixel.green = 0;
        blue_pixel.blue = 255;

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
	TickType_t curr_time, prev_time;

	curr_time = xTaskGetTickCount();
	prev_time = xTaskGetTickCount();

    initGame();
	while(1) {
		curr_time =  xTaskGetTickCount();

		updateFrameBuffer();
		examineButtons();
		if((prev_time + SNAKE_SPEED_TICKS) < curr_time){
			moveSnake();
			prev_time = curr_time;
		}

		vTaskDelay(33);
	}
}
	
