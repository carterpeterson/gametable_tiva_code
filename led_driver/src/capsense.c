#include "capsense.h"

void init_capsense_gpio(void)
{
	gpio_digital_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_alternate_function_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_config_port_ctl(PORT_A, (PIN_6 | PIN_7), 3);
	gpio_open_drain_enable(PORT_A, PIN_7);
}

void init_capsense_i2c(void)
{
	initializeI2CMaster(I2C1_BASE);
	
}

void init_capsense(void)
{
	i2c_status_t status;
	uint8_t data;

	init_capsense_gpio();
	init_capsense_i2c();
	
	while(1) {
		i2cSetSlaveAddr(I2C1_BASE, 0x37, I2C_WRITE);
		do {
			status = i2cSendByte(I2C1_BASE, 0xAE, I2C_MCS_START | I2C_MCS_RUN);
		} while(status != I2C_OK);
		
		i2cSetSlaveAddr(I2C1_BASE, 0x37, I2C_READ);
		
		status = i2cGetByte(I2C1_BASE, &data, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
		
		if(status & 0x01) {
			read_buffer[0].red = 255;
			read_buffer[0].blue = 0;
			read_buffer[0].green = 0;
		} else {
			read_buffer[0].red = 0;
			read_buffer[0].blue = 0;
			read_buffer[0].green = 0;
		}
		
		convert_buffer();
	}
}
