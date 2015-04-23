#include "capsense.h"

typedef struct {
	uint8_t device_address;
	uint8_t bit_0_pixel, bit_1_pixel;
} Touch_pixel_mapping;

typedef struct {
	uint8_t current_device;
	bool complete;
	I2C_channel *section_channel;
	uint16_t buffers[2];
	Touch_pixel_mapping mappings[CAPSENSE_CONTROLLERS_PER_SECTION];
} Touch_section;

static Touch_section section_one = {
	0x00,
	false,
	&i2c1,
	{0, 0},
	{{0x20, 0, 1}, {0x21, 16, 17}, {0x22, 2, 3}, {0x23, 18, 19},
	{0}, {0}, {0}, {0},
	{0}, {0}, {0}, {0},
	{0}, {0}, {0}, {0}}
};

static Touch_section touch_sections[ACTIVE_SECTIONS_CAPSENSE];

uint32_t touch_buffers[2][4] = {0};
uint32_t *touch_read_buffer, *touch_write_buffer;

void init_touch_sections(void)
{
	touch_sections[0] = section_one;
}

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

bool all_sections_complete(void)
{
	int i = 0;
	for(; i < ACTIVE_SECTIONS_CAPSENSE; i++) {
		if(!touch_sections[i].complete)
			return false;
	}
	
	return true;
}

void transmit_touch_buffer(void)
{
	printf(":)\n\r");
}

void capsense_write(Touch_section section)
{
	i2c_slave_address_set(section.section_channel->channel, section.mappings[section.current_device].device_address);
	i2c_slave_rw_set(section.section_channel->channel, I2C_MSA_SLAVE_WRITE);
	section.section_channel->channel->MDR = CAPSENSE_PROX_REG_ADDR;
	section.section_channel->read_req = false;
	section.section_channel->channel->MCS = I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP;
}

void capsense_read(Touch_section section)
{
	i2c_slave_address_set(section.section_channel->channel, section.mappings[section.current_device].device_address);
	i2c_slave_rw_set(section.section_channel->channel, I2C_MSA_SLAVE_READ);
	section.section_channel->read_req = true;
	section.section_channel->channel->MCS = I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP;
}

void process_capsense_section(Touch_section section)
{
	uint8_t data, pixel;

	if(section.complete || !section.section_channel->update_pending)
		return;
		
	if(section.section_channel->status & (I2C_MCS_ARB_LOST | I2C_MCS_DATA_ACK | I2C_MCS_ADDR_ACK | I2C_MCS_ERROR | I2C_MCS_BUSY | I2C_MCS_BUS_BUSY)) {
		
		if(section.section_channel->attempt < MAX_TRIES_I2C) {
			section.section_channel->attempt++;
			
			if(section.section_channel->read_req)
				capsense_read(section);
			else
				capsense_write(section);
		} else { // Failed request
			section.section_channel->attempt = 0;
			section.current_device++;
			
			while(section.mappings[section.current_device].device_address == 0x00
				&& section.current_device < CAPSENSE_CONTROLLERS_PER_SECTION)
				section.current_device++;
			
			if(section.current_device >= CAPSENSE_CONTROLLERS_PER_SECTION) {
				section.complete = true;
			} else {
				capsense_write(section);
			}
		}
	} else {
		if(section.section_channel->read_req) {
			// Save off the data
			data = section.section_channel->channel->MDR;
			
			if((data & BIT_0) == BIT_0) {
				pixel = section.mappings[section.current_device].bit_0_pixel;
				section.buffers[pixel / 16] |= (0x01 << (15 - (pixel % 16)));
			}
			
			if((data & BIT_1) == BIT_1) {
				pixel = section.mappings[section.current_device].bit_1_pixel;
				section.buffers[pixel / 16] |= (0x01 << (15 - (pixel % 16)));
			}
			
			// Start the next section
			section.current_device++;
			
			while(section.mappings[section.current_device].device_address == 0x00
				&& section.current_device < CAPSENSE_CONTROLLERS_PER_SECTION)
				section.current_device++;
			
			if(section.current_device >= CAPSENSE_CONTROLLERS_PER_SECTION) {
				section.complete = true;
			} else {
				capsense_write(section);
			}
		} else {
			capsense_read(section);
		}
	}
	
	section.section_channel->update_pending = false;
}

void reset_touch_sections(void)
{
	int i = 0;
	for(; i < ACTIVE_SECTIONS_CAPSENSE; i++) {
		touch_sections[i].complete = false;
		touch_sections[i].buffers[0] = 0;
		touch_sections[i].buffers[1] = 0;
		touch_sections[i].current_device = 0;
		touch_sections[i].section_channel->attempt = 0;
		capsense_write(touch_sections[i]);
	}
}

void init_capsense(void)
{	
	int i;

	init_capsense_gpio();
	init_capsense_i2c();
	init_capsense_buffers();
	reset_touch_sections();
	
	while(1) {
		if(all_sections_complete()) {
			transmit_touch_buffer();
			reset_touch_sections();
		} else {
			for(i = 0; i < ACTIVE_SECTIONS_CAPSENSE; i++) {
				process_capsense_section(touch_sections[i]);
			}
		}
	}
}
