#include "capsense.h"

typedef struct {
	uint8_t device_address;
	uint8_t sensor_bit_mask;
} Touch_pixel_mapping;

typedef struct {
	I2C_channel *section_channel;
	Touch_pixel_mapping mappings[PIXELS_PER_TOUCH_SECTION];
} Touch_section;

static Touch_section section_one = {
	&i2c1,
	{{0x20, 0x01}, {0x20, 0x02}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, 
	{0x21, 0x02}, {0x21, 0x01}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, }
};

uint32_t touch_buffers[2][4] = {0};
uint32_t *touch_read_buffer, *touch_write_buffer;

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

void init_capsense_buffers(void)
{
	touch_read_buffer = touch_buffers[0];
	touch_write_buffer = touch_buffers[1];
}

void display_touches(void)
{
	Pixel red_p, *temp;
	int i, j;
	
	red_p.red = 255;
	
	write_buffer[0].red = 0;
	write_buffer[1].red = 0;
	write_buffer[2].red = 0;
	write_buffer[3].red = 0;
	
	for(i = 0; i < 2; i++) {
		printf("%d\n\r", touch_read_buffer[i]);
	
		for(j = 0; j < 2; j++) {
			if((touch_read_buffer[i] >> (31 - j)) & 0x01) {
				if(i == 0 && j == 0) {
					write_buffer[1] = red_p;
				} else if(i == 0 && j == 1) {
					write_buffer[2] = red_p;
				} else if(i == 1 && j == 0) {
					write_buffer[0] = red_p;
				} else if(i == 1 && j == 1) {
					write_buffer[3] = red_p;
				}
			}
		}
	}
	
	temp = write_buffer;
	write_buffer = read_buffer;
	read_buffer = temp;
		
	convert_buffer();
}

void init_capsense(void)
{
	uint32_t *temp;
	int wait, i;
	uint8_t sensor_data;
	I2C_request write_req_0, read_req_0, write_req_1, read_req_1, *cur_parse_req;
	
	init_capsense_gpio();
	init_capsense_i2c();
	init_capsense_buffers();
	
	write_req_0.read_req = false;
	write_req_0.complete = false;
	write_req_0.dependent = false;
	write_req_0.device_addr = 0x20;
	write_req_0.data = 0xAE;
	write_req_0.size = 1;
	write_req_0.next_req = &read_req_0;
	
	read_req_0.read_req = true;
	read_req_0.complete = false;
	read_req_0.dependent = true;
	read_req_0.device_addr = 0x20;
	read_req_0.data = 0;
	read_req_0.size = 1;
	read_req_0.next_req = &write_req_1;
	
	write_req_1.read_req = false;
	write_req_1.complete = false;
	write_req_1.dependent = false;
	write_req_1.device_addr = 0x21;
	write_req_1.data = 0xAE;
	write_req_1.size = 1;
	write_req_1.next_req = &read_req_1;
	
	read_req_1.read_req = true;
	read_req_1.complete = false;
	read_req_1.dependent = true;
	read_req_1.device_addr = 0x21;
	read_req_1.data = 0;
	read_req_1.size = 1;
	read_req_1.next_req = NULL;//&write_req_1;
	
	i2c_handle_request(&i2c1, &write_req_0);
	
	while(1) {
		if(i2c1.update_pending) {
			i2c_handle_response(&i2c1);
		} else if (i2c1.busy == false) {
			wait = 0;
			while(wait < 3000)
				wait++;
				
			touch_write_buffer[0] = 0;
			touch_write_buffer[1] = 0;
				
			for(i = 0; i < PIXELS_PER_TOUCH_SECTION; i++) {
				
				cur_parse_req = &read_req_0;
				
				if(section_one.mappings[i].device_address != 0x00) {
				
					while((section_one.mappings[i].device_address != cur_parse_req->device_addr || !cur_parse_req->read_req)
							&& cur_parse_req->next_req != NULL) {
							cur_parse_req = cur_parse_req->next_req;
					}

					if(cur_parse_req->device_addr == section_one.mappings[i].device_address
						&& cur_parse_req->read_req) {
					
						sensor_data = (section_one.mappings[i].sensor_bit_mask & cur_parse_req->data) ? 0x1 : 0x0;
						touch_write_buffer[i / 16] |= (sensor_data << (31 - (i % 16)));
					}
				}
			}
			
			temp = touch_read_buffer;
			touch_read_buffer = touch_write_buffer;
			touch_write_buffer = temp;
				
			display_touches();	
				
			read_req_0.data = 0;
			read_req_1.data = 0;
			i2c_handle_request(&i2c1, &write_req_0);
		}
	}
}
