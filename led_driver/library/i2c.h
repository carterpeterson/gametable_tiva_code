#ifndef __I2C_H__
#define __I2C_H__

// C
#include <stdint.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"

// Project
#include "config.h"
#include "uart.h"


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

// Timer Periods, these assume we're running at 80 MHz
#define I2C_TIMER_PERIOD_100K	0x27
#define I2C_TIMER_PERIOD_400K 0x09

//	Master Slave Address
#define I2C_MSA_ADDR_MASK			0xFE
#define I2C_MSA_RW_MASK				0x01
#define I2C_MSA_SLAVE_WRITE		0x00
#define I2C_MSA_SLAVE_READ		0x01
#define I2C_MSA_ADDR_SHAMT		0x01

// Master Control / Status
//	Reading
#define	I2C_MCS_CLOCK_TO			0x80
#define	I2C_MCS_BUS_BUSY			0x40
#define	I2C_MCS_IDLE					0x20
#define	I2C_MCS_ARB_LOST			0x10
#define I2C_MCS_DATA_ACK			0x08
#define I2C_MCS_ADDR_ACK			0x04
#define I2C_MCS_ERROR					0x02
#define I2C_MCS_BUSY					0x01

//	Writing
#define I2C_MCS_HS_EN					0x10
#define I2C_MCS_ACK_EN				0x08
#define I2C_MCS_STOP					0x04
#define	I2C_MCS_START					0x02
#define	I2C_MCS_RUN						0x01

bool i2c_enable(uint8_t cgc_mask);
void i2c_init_master(I2C0_Type* i2c);
void i2c_init_slave(I2C0_Type* i2c);
void i2c_config_speed(I2C0_Type* i2c, uint8_t speed);
void i2c_slave_address_set(I2C0_Type* i2c, uint8_t addr);
void i2c_slave_rw_set(I2C0_Type* i2c, uint8_t rw_mask);
bool i2c_send_byte(I2C0_Type* i2c, uint8_t data, bool stop, bool repeat_start);
bool i2c_read_byte(I2C0_Type* i2c, uint8_t *data, bool stop, bool repeat_start);

#endif
