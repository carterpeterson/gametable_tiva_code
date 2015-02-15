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
#define TIMER_0_CGC		0x01
#define TIMER_1_CGC		0x02
#define TIMER_2_CGC		0x04
#define TIMER_3_CGC		0x08
#define TIMER_4_CGC		0x10
#define TIMER_5_CGC		0x20

#define TIMER_A_ENABLE 	0x0001
#define TIMER_B_ENABLE 	0x0100

#define TIMER_CFG_FULL	0x0
#define TIMER_CFG_RTC	0x1
#define TIMER_CFG_HALF	0x4

#define TIMER_MODE_ONESHOT	0x1
#define TIMER_MODE_PERIODIC	0x2
#define TIMER_MODE_CAPTURE	0x3

bool timer_enable(uint8_t timers);
bool timer_periodic_enable(TIMER0_Type *timer);

#endif
