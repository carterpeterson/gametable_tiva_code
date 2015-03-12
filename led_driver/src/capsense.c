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
	i2c_enable(I2C1_CGC);
	i2c_init_master(I2C1);
	i2c_master_interrupt_enable(I2C1, 1);
}

uint8_t get_proximity(I2C0_Type *channel, uint8_t address)
{
	bool pass;
	uint8_t data, retry_count = 0;
	
	i2c_slave_address_set(channel, address);
	
	i2c_slave_rw_set(channel, I2C_MSA_SLAVE_WRITE);
		
	do {
		pass = i2c_send_byte(I2C1, 0xAE, true, false);
		retry_count++;
	} while(pass != true && retry_count <= 5);
	
	/*if(pass != true)
		return 0xFF;
	
	i2c_slave_rw_set(channel, I2C_MSA_SLAVE_READ);
	pass = i2c_read_byte(channel, &data, true, false);
	
	if(pass != true)
		return 0xFF;*/
	
	return data;
}

void init_capsense(void)
{
	uint8_t data;
	int wait;
	I2C_request req;
	
	init_capsense_gpio();
	init_capsense_i2c();
	
	
	while(1) {
		
		//data = get_proximity(I2C1, 0x37);
		req.device_addr = 0x37;
		req.read_req = false;
		req.data = 0xAE;
		req.size = 1;
		
		i2c_handle_request(&i2c1, &req);

		wait = 0;
		
		while(1)
			wait++;
			//count++;
		
		/*if(data != 0xFF) {			
			if(data & 0x01) {
				read_buffer[0].red = 255;
				read_buffer[0].blue = 0;
				read_buffer[0].green = 0;
				printf("0\n\r");
			} else {
				read_buffer[0].red = 0;
				read_buffer[0].blue = 0;
				read_buffer[0].green = 0;
				printf(" \n\r");
			}
			
			//convert_buffer();
		}*/
	}
}
