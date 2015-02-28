#ifndef __I2C_H__
#define __I2C_H__

// C
#include <stdint.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"

// Project
#include "./config.h"


/*
*		I2C related defines
*/

//	Clock Gating
#define I2C0_CGC		0x01
#define I2C1_CGC		0x02
#define I2C2_CGC		0x04
#define I2C3_CGC		0x08
#define INVALID_CGC	0xF0

// Configuration Register
#define I2C_MCR_GLITCH_FILTER	0x40
#define I2C_MCR_SLAVE_EN			0x20
#define I2C_MCR_MASTER_EN			0x10
#define I2C_MCR_LOOPBACK			0x01

// Timer Periods
#define I2C_TIMER_PERIOD_100K	0x27
#define I2C_TIMER_PERIOD_400K 0x09


bool i2c_enable(uint8_t cgc_mask);

#endif
