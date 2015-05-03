#include "pong.h"

int moveBall(void);
void initializeGame(void);
void movePaddles(void);
void updateScreen(void);


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
extern int next;

extern Pixel green_pixel;
extern Pixel red_pixel;
extern Pixel blank_pixel;
extern Pixel blue_pixel;

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

void launchBall(bool hold){

    int x,y;

	ballX = GRIDX/2;
	ballY = GRIDY/2;
	pongGrid[ballX][ballY] = BALL;
	
    while(1){
    	updateScreen();
		vTaskDelay(1000);
        if(is_pixel_touched(ballX,ballY) || !hold){
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
		vTaskDelay(TICK_DELAY_30_FPS);
    }
    

}

void finishGame(int winner){

    int x, y; 
    int counter = 0;
    
    for(x = 0; x < GRIDX; x++){
        for(y = 0; y < GRIDY; y++){
            pongGrid[x][y] = BLANKSPACE;
        }
    }
    
    while(1){
        for(x = 0; x < 10; x++){
            for(y = 0; y < GRIDY; y++){
                if(winner == LPAD){  
                    pongGrid[x + 22][y] = (y + counter) % 3;
                }
                else{
                    pongGrid[x][y] = (y + counter) % 3;
                }
            }
        }
        
        ballX = GRIDX/2;
	    ballY = GRIDY/2;
	    pongGrid[ballX][ballY] = BALL;
	    
        updateScreen();
        counter++;
        vTaskDelay(50);
        if(is_pixel_touched(ballX,ballY)){
            return;
        }
       
    }
}

int moveBall(void){	
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
            return LPAD;
	    }
	    else{
	        updateScreen();
	        launchBall(false);
	    }
	}
	else if(ballX + ballDir[0] >= GRIDX){
        rScore++;
	    if(rScore >= 10){
            return RPAD;
	    }
	    else{
	        updateScreen();
	        launchBall(false);
	    }
	}
	return 0;
}

void movePaddles(void)
{  
	bool foundL, foundR;
	int i;
	
	foundL = false;
	foundR = false;
	
	//if button has been pressed determine which has bee n pressed
	//and set Direction var to appropriate value
	for(i = 0; i < GRIDY; i++){
	    pongGrid[0][i] = BLANKSPACE;
	    pongGrid[GRIDX-1][i] = BLANKSPACE;
    }

    for(i = 0; i < GRIDY; i++){
        if(is_pixel_touched(0,i) && !foundL){
            pongGrid[0][i] = LPAD;
            if(i+1<GRIDY){
                pongGrid[0][i+1] = LPAD;
            }
            
            if(i-1 >= 0){
                pongGrid[0][i-1] = LPAD;
            }
			foundL = true;
        }
        
        if(is_pixel_touched(GRIDX-1,i)){
            pongGrid[GRIDX-1][i] = RPAD;
            if(i+1<GRIDY){
                pongGrid[GRIDX-1][i+1] = RPAD;
            }
            
            if(i-1 >= 0){
                pongGrid[GRIDX-1][i-1] = RPAD;
            }
			foundR = true;
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
    int game_state;
    TickType_t curr_time, prev_time;
    
    initializeGame();
    launchBall(true);
   
    curr_time = xTaskGetTickCount();
    prev_time = xTaskGetTickCount();

    while(1) {
        curr_time = xTaskGetTickCount();
        updateScreen();
        movePaddles();
        if(prev_time + BALL_SPEED_TICKS < curr_time){
            if((game_state = moveBall()) != 0){
                finishGame(game_state);
                initializeGame();
                launchBall(true);
                
                curr_time = xTaskGetTickCount();
                prev_time = xTaskGetTickCount();
            }
            prev_time = curr_time;
        }
        updateScreen();
		vTaskDelay(TICK_DELAY_30_FPS);
	}
}
	
