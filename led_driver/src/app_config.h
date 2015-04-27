#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#define STRING_LENGTH_PIXEL 	32
#define STRING_LENGTH_COLOR		96
#define STRING_LENGTH_BITS		768

#define TOTAL_PIXELS			256

#define INPUT_BUFF_MINUS_ONE	767U

#define ACTIVE_SECTIONS_LED 		8
#define ACTIVE_SECTIONS_CAPSENSE 	4
#define WIDTH_PIXELS 		32

//#define LED_BOARD
//#define USING_SIMULATOR

#define I2C_CLOCK_LOW_TIMEOUT		0x1f // 0x1f0 (0 added by hardware) gives roughly 5 ms

#endif
