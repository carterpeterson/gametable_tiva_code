#ifndef __TIMER_H__
#define __TIMER_H__

// C
#include <stdbool.h>
#include <stdint.h>

// MCU
#include "TM4C123.h"

// Project
#include "config.h"

// Timer related defines

// Clock Gating
#define TIMER_0_CGC				0x01
#define TIMER_1_CGC				0x02
#define TIMER_2_CGC				0x04
#define TIMER_3_CGC				0x08
#define TIMER_4_CGC				0x10
#define TIMER_5_CGC				0x20
#define TIMER_INVALID_CGC 0xC0

/*
*		Configuration: Determines whether the GPTM is in:
*			32/64, 16/32, or Real Time Clock mode.
*/
#define TIMER_CFG_FULL	0x0
#define TIMER_CFG_RTC		0x1
#define TIMER_CFG_HALF	0x4


/*
*		Timer Modes
*/
#define TIMER_TAPLO			(0x1 << 11)	// Legacy operation
#define TIMER_TAMRSU		(0x1 << 10)	// Match Register Update
#define TIMER_TAPWMIE		(0x1 << 9)	// PWM Interrupt Enable
#define TIMER_TALID			(0x1 << 8)	// Interval Load Write
#define TIMER_TASNAPS		(0x1 << 7)	// Snap-Shot Mode
#define TIMER_TAWOT			(0x1 << 6)	// Wait-on-Trigger
#define TIMER_TAMIE			(0x1 << 5)	// Match Interrupt Enable
#define TIMER_TACDIR		(0x1 << 4)	// Count Direction
#define TIMER_TAAMS			(0x1 << 3)	// Alternate Mode Select
#define TIMER_TACMR			(0x1 << 2)	// Capture Mode
#define TIMER_MODE_ONE_SHOT	0x1			// One-Shot Timer mode
#define TIMER_MODE_PERIODIC 0x2			// Periodic Timer mode
#define TIMER_MODE_CAPTURE	0x3			// Capture mode
#define TIMER_MODE_CLEAR		0x3			// Capture mode

/*
*		Control
*/
#define TIMER_CTL_B_PWML 				(0x1 << 14)	// Timer B PWM Output Level
#define TIMER_CTL_B_OTE 				(0x1 << 13)	// Timer B Output Trigger Enable
#define TIMER_CTL_B_EVENT_POS 	(0x0 << 10)	// Timer B Event Positive Edge
#define TIMER_CTL_B_EVENT_NEG 	(0x1 << 10)	// Timer B Event Negative Edge
#define TIMER_CTL_B_EVENT_BOTH 	(0x3 << 10)	// Timer B Event Both Edges
#define TIMER_CTL_B_STALL 			(0x1 << 9)	// Timer B Stall Enable
#define TIMER_CTL_B_EN 	 				(0x1 << 8)	// Timer B Enable
#define TIMER_CTL_A_PWML 				(0x1 << 6)	// Timer A PWM Output Level
#define TIMER_CTL_A_OTE 				(0x1 << 5)	// Timer A Output Trigger Enable
#define TIMER_CTL_A_RTCEN 	 		(0x1 << 4)	// Timer A RTC Stall Enable
#define TIMER_CTL_A_EVENT_POS 	(0x0 << 2)	// Timer A Event Positive Edge
#define TIMER_CTL_A_EVENT_NEG 	(0x1 << 2)	// Timer A Event Negative Edge
#define TIMER_CTL_A_EVENT_BOTH 	(0x3 << 2)	// Timer A Event Both Edges
#define TIMER_CTL_A_STALL 			(0x1 << 1)	// Timer A Stall Enable
#define TIMER_CTL_A_EN 	 				(0x1 << 0)	// Timer A Enable

/*
*		Synchronize
*/
// TODO: Add synchronize defines


/*
*		Interrupt Mask
*/



/*
*		Interface defines. These help for easy function calls.
*/


/*
*		Control
*/
#define TIMER_ENABLE_A		TIMER_CTL_A_EN
#define TIMER_ENABLE_B		TIMER_CTL_B_EN
#define TIMER_ENABLE_BOTH	0x00000101UL


bool timer_init(TIMER0_Type *timer);
bool timer_periodic_enable(TIMER0_Type *timer);

#endif
