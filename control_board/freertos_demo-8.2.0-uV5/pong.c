#include "pong.h"

//game matrix
int pongGrid[GRIDX][GRIDY];

//ball location
uint8_t ballX;
uint8_t ballY;

//location of middle pixel of each paddle
uint8_t lPadY;
uint8_t rPadY;

uint16_t lScore;
uint16_t rScore;
int ballDir[2];
int next;

bool gameOver;

Pixel green_pixel;
Pixel red_pixel;
Pixel blank_pixel;
Pixel blue_pixel;
        
void moveBall(void);
void initializeGame(void);
void movePaddles(void);
void updateScreen(void);

void initializeGame(void){
	//initialize game variables
	int x, y; 
	    
    for(x = 0; x < GRIDX; x++){
        for(y = 0; y < GRIDY; y++){
            pongGrid[x][y] = BLANKSPACE;
        }
    }
	
	
    lPadY = GRIDY/2;
    lScore = 0;
    pongGrid[0][lPadY] = LPAD;
    pongGrid[0][lPadY+1] = LPAD;
    pongGrid[0][lPadY-1] = LPAD;
    
    rPadY = GRIDY/2;
	rScore = 0;
	pongGrid[GRIDX-1][rPadY] = RPAD;
    pongGrid[GRIDX-1][rPadY+1] = RPAD;
    pongGrid[GRIDX-1][rPadY-1] = RPAD;

	gameOver = false;
	
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

void launchBall(){

    int x,y;
    for(x = 0; x < GRIDX; x++){
        for(y = 0; y < GRIDY; y++){
            pongGrid[x][y] = BLANKSPACE;
        }
    }

	ballX = GRIDX/2;
	ballY = GRIDY/2;
	pongGrid[ballX][0] = BALL;
	
	updateScreen();
	
    while(1){
        if(is_pixel_touched(ballX,ballY)){
            int r = rand() % (4);
            switch(r){
                case 0:
                    ballDir[0] = -1;
                    ballDir[1] = -1;
                break;
                case 1:
                    ballDir[0] = 1;
                    ballDir[1] = -1;
                break;
                case 2:
                    ballDir[0] = -1;
                    ballDir[1] = 1;
                break;
                case 3:
                    ballDir[0] = 1;
                    ballDir[1] = 1;
                break;
            }
            break;
        }
    }
    

}

void finishGame(int winner){
    /*printf("Game Over\n");
    printf("%d, %d\n", lScore, rScore);
    if(winner == LPAD){
        printf("Left Player Wins!\n");
    }
    else if(winner == RPAD){
        printf("Right Player Wins!\n");    
    }*/
    gameOver = true;
}

void moveBall(void){	
	//set previous head spot with direction the head is about to travel
	pongGrid[ballX][ballY] = BLANKSPACE;
	
	if(ballY + ballDir[1] < 0){
	    ballDir[1] = DOWN;
	}
	else if(ballY + ballDir[1] >= GRIDY){
	    ballDir[1] = UP;
	}
	
	if(ballX + ballDir[0] == 0){
	    if(pongGrid[ballX + ballDir[0]][ballY] == LPAD){
	        ballDir[0] = RIGHT;
	    }
	}
	else if(ballX + ballDir[0] == GRIDX - 1){
	    if(pongGrid[ballX + ballDir[0]][ballY] == RPAD){
	        ballDir[0] = LEFT;
	    }	
	}
	
	ballY += ballDir[1];
    ballX += ballDir[0];
	
	
	//look at the spot we are about to move the head
	pongGrid[ballX][ballY] = BALL;
	
	if(ballX + ballDir[0] < 0){
	    lScore++;

	    if(lScore >= 10){
	        finishGame(LPAD);
	    }
	    else{
	        updateScreen();
	        launchBall();
	    }
	    return;
	}
	else if(ballX + ballDir[0] >= GRIDX){
        rScore++;
	    if(rScore >= 10){
	        finishGame(RPAD);
	    }
	    else{
	        //printf("%d, %d\n", lScore, rScore);
	        updateScreen();
	        launchBall();
	    }
	    return;
	}
	
}

void movePaddles(void)
{  
	
	int i;
	
	//if button has been pressed determine which has bee n pressed
	//and set Direction var to appropriate value
	for(i = 0; i < GRIDY; i++){
	    pongGrid[0][i] = BLANKSPACE;
	    pongGrid[GRIDX-1][i] = BLANKSPACE;
    }

    for(i = 0; i < GRIDY; i++){
        if(is_pixel_touched(0,i)){
            pongGrid[0][i] = LPAD;
            if(i+1<GRIDY){
                pongGrid[0][i+1] = LPAD;
            }
            
            if(i-1 >= 0){
                pongGrid[0][i-1] = LPAD;
            }
        }
        
        if(is_pixel_touched(GRIDX-1,i)){
            pongGrid[GRIDX-1][i] = RPAD;
            if(i+1<GRIDY){
                pongGrid[GRIDX-1][i+1] = RPAD;
            }
            
            if(i-1 >= 0){
                pongGrid[GRIDX-1][i-1] = RPAD;
            }
        }
    }
	
	return;
}

void updateScreen(void){

    int x,y;
    
    for(x = 0; x < GRIDX; x++){
        for(y = 0; y < GRIDY; y++){
            if(pongGrid[x][y] == BALL){
                frame_buffer[y*GRIDX + x] = red_pixel;
            }
            else if(pongGrid[x][y] == LPAD){
                frame_buffer[y*GRIDX + x] = green_pixel;
            }
            else if(pongGrid[x][y] == RPAD){
                frame_buffer[y*GRIDX + x] = blue_pixel;
            }
            else{
                frame_buffer[y*GRIDX + x] = blank_pixel;
            }
        }
    }
    render();
}

void task_pong_game(void *pvParameters)
{
	TickType_t curr_time, prev_time;
    
    initializeGame();
    updateScreen();
    launchBall();

    curr_time = xTaskGetTickCount();
	prev_time = xTaskGetTickCount();

    while(1) {
		curr_time = xTaskGetTickCount();
		
		updateScreen();
		movePaddles();
		
		if((prev_time + 100) < curr_time){
			moveBall();
			if(gameOver){
				break;
			}
			prev_time = curr_time;
		}
		updateScreen();
		vTaskDelay(33);
	
  }
}
	
