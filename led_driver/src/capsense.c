#include "capsense.h"

void init_capsense_gpio(void)
{
	//gpio_digital_enable(PORT_A, (PIN_6 | PIN_7));
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

/*void init_capsense(void)
{
	int wait;
	I2C_request write_req, read_req;
	
	//init_capsense_gpio();
	gpio_alternate_function_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_config_port_ctl(PORT_A, (PIN_6 | PIN_7), 3);
	gpio_open_drain_enable(PORT_A, PIN_7);
	//init_capsense_i2c();
	i2c_enable(I2C1_CGC);
	i2c_init_master(I2C1);
	i2c_master_interrupt_enable(I2C1, 1);
	
	
	write_req.device_addr = 0x20;
	write_req.read_req = false;
	write_req.data = 0xAE;
	write_req.size = 1;
	write_req.next_req = &read_req;
	
	read_req.device_addr = 0x20;
	read_req.read_req = true;
	read_req.size = 1;
	read_req.next_req = 0;
	read_req.data = 0;
	
	i2c_handle_request(&i2c1, &write_req);
		
	while(1) {
		if(i2c1.update_pending) {
			i2c_handle_response(&i2c1);
		} else if (i2c1.busy == false) {
			wait = 0;
			while(wait < 3000000)
				wait++;
				
			read_req.data = 0;	
			i2c_handle_request(&i2c1, &write_req);
		}
	}
}*/
