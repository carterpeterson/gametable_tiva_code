#include "i2c.h"

static bool has_stopped = true;

bool i2c_enable(uint8_t cgc_mask)
{
	if((cgc_mask & INVALID_CGC) != 0x00)
		return false;

	SYSCTL->RCGCI2C |= cgc_mask;
	
	while((SYSCTL->PRI2C & cgc_mask) != cgc_mask)
		; // Wait
	
	return true;
}

void i2c_init_master(I2C0_Type* i2c)
{
	i2c->MCR |= I2C_MCR_MASTER_EN;
}

void i2c_init_slave(I2C0_Type* i2c)
{
	i2c->MCR |= I2C_MCR_SLAVE_EN;
}

void i2c_config_speed(I2C0_Type* i2c, uint8_t speed)
{
	i2c->MTPR &= ~(0x7F);
	i2c->MTPR |= speed;
}

void i2c_slave_address_set(I2C0_Type* i2c, uint8_t addr)
{
	uint8_t data = i2c->MSA;
	data &= ~(I2C_MSA_ADDR_MASK);
	i2c->MSA = data | (addr << I2C_MSA_ADDR_SHAMT);
}

void i2c_slave_rw_set(I2C0_Type* i2c, uint8_t rw_mask)
{
	i2c->MSA &= ~(I2C_MSA_RW_MASK);
	i2c->MSA |= rw_mask;
}

bool i2c_send_byte(I2C0_Type* i2c, uint8_t data, bool stop, bool repeat_start)
{
	uint8_t command = I2C_MCS_RUN;
	
	i2c->MDR = data;
	
	if(has_stopped || repeat_start) {
		command |= I2C_MCS_START;
	}
	
	if(stop) {
		command |= I2C_MCS_STOP;
		has_stopped = true;
	} else {
		has_stopped = false;
	}
	
	i2c->MCS = command;
	
	while(i2c->MCS & I2C_MCS_BUSY)
		; // Do nothing
	
	//printf("%d\n\r", wasted);
	
	if(i2c->MCS & (I2C_MCS_ARB_LOST | I2C_MCS_DATA_ACK | I2C_MCS_ADDR_ACK | I2C_MCS_ERROR)) {
		i2c->MCS = I2C_MCS_STOP;
		return false;
	}
	
	return true;
}

bool i2c_read_byte(I2C0_Type* i2c, uint8_t *data, bool stop, bool repeat_start)
{
	uint8_t command = I2C_MCS_RUN;
	
	if(has_stopped || repeat_start) {
		command |= I2C_MCS_START;
	}
	
	if(stop) {
		command |= I2C_MCS_STOP;
		has_stopped = true;
	} else {
		has_stopped = false;
	}
	
	i2c->MCS = command;
	while(i2c->MCS & I2C_MCS_BUSY)
		;	// Do nothing
	
	if(i2c->MCS & (I2C_MCS_ERROR))
		return false;
	
	*data = i2c->MDR;
	return true;
}
