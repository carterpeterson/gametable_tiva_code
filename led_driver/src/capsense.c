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

static Touch_section touch_sections[ACTIVE_SECTIONS_CAPSENSE] =
{
	{0x00, false, &i2c1, {0, 0},
		{{0x20, 14, 15}, {0x21, 31, 30}, 
		{0x22, 12, 13}, {0x23, 29, 28},
		{0x24, 10, 11}, {0x25, 27, 26},
		{0x26, 8, 9}, {0x27, 25, 24},
		{0x28, 6, 7}, {0x29, 23, 22}, 
		{0x2A, 4, 5}, {0x2B, 21, 20},
		{0x2C, 2, 3}, {0x2D, 19, 18},
		{0x2E, 0, 1}, {0x2F, 17, 16}}},

	{0x00, false, &i2c2, {0, 0},
		{{0x20, 17, 16}, {0x21, 0, 1}, 
		{0x22, 19, 18}, {0x23, 2, 3},
		{0x24, 21, 20}, {0x25, 4, 5},
		{0x26, 23, 22}, {0x27, 6, 7},
		{0x28, 25, 24}, {0x29, 8, 9}, 
		{0x2A, 27, 26}, {0x2B, 10, 11},
		{0x2C, 29, 28}, {0x2D, 12, 13},
		{0x2E, 31, 30}, {0x2F, 14, 15}}},
	{0x00, false, &i2c3, {0, 0},
		{{0x20, 17, 16}, {0x21, 0, 1}, 
		{0x22, 19, 18}, {0x23, 2, 3},
		{0x24, 21, 20}, {0x25, 4, 5},
		{0x26, 23, 22}, {0x27, 6, 7},
		{0x28, 25, 24}, {0x29, 8, 9}, 
		{0x2A, 27, 26}, {0x2B, 10, 11},
		{0x2C, 29, 28}, {0x2D, 12, 13},
		{0x2E, 31, 30}, {0x2F, 14, 15}}},
	{0x00, false, &i2c0, {0, 0},
		{{0x20, 14, 15}, {0x21, 31, 30}, 
		{0x22, 12, 13}, {0x23, 29, 28},
		{0x24, 10, 11}, {0x25, 27, 26},
		{0x26, 8, 9}, {0x27, 25, 24},
		{0x28, 6, 7}, {0x29, 23, 22}, 
		{0x2A, 4, 5}, {0x2B, 21, 20},
		{0x2C, 2, 3}, {0x2D, 19, 18},
		{0x2E, 0, 1}, {0x2F, 17, 16}}}
};

uint32_t touch_buffers[2][TOUCH_BUFFER_SIZE] = {0};
uint32_t *touch_read_buffer, *touch_write_buffer;
bool pushing_uart = false;
bool capsense_poll_requested = false;
bool polling_capsense = false;

void init_capsense_gpio(void)
{
	// I2C0
	gpio_port_enable(PORT_B_CGC);
	gpio_digital_enable(PORT_B, (PIN_2 | PIN_3));
	gpio_alternate_function_enable(PORT_B, (PIN_2 | PIN_3));
	gpio_config_port_ctl(PORT_B, (PIN_2 | PIN_3), 3);
	gpio_open_drain_enable(PORT_B, PIN_3);
	
	// I2C1
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_alternate_function_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_config_port_ctl(PORT_A, (PIN_6 | PIN_7), 3);
	gpio_open_drain_enable(PORT_A, PIN_7);
	
	// I2C2
	gpio_port_enable(PORT_E_CGC);
	gpio_digital_enable(PORT_E, (PIN_4 | PIN_5));
	gpio_alternate_function_enable(PORT_E, (PIN_4 | PIN_5));
	gpio_config_port_ctl(PORT_E, (PIN_4 | PIN_5), 3);
	gpio_open_drain_enable(PORT_E, PIN_5);
	
	// I2C3
	gpio_port_enable(PORT_D_CGC);
	gpio_digital_enable(PORT_D, (PIN_0 | PIN_1));
	gpio_alternate_function_enable(PORT_D, (PIN_0 | PIN_1));
	gpio_config_port_ctl(PORT_D, (PIN_0 | PIN_1), 3);
	gpio_open_drain_enable(PORT_D, PIN_1);
	
	#ifndef USING_SIMULATOR
		// UART 2
		gpio_port_enable(PORT_D_CGC);
		gpio_digital_enable(PORT_D, (PIN_6 | PIN_7));
		gpio_pin_direction(PORT_D, DIRECTION_OUTPUT, PIN_7);
		gpio_pin_direction(PORT_D, DIRECTION_INPUT, PIN_6);
		gpio_alternate_function_enable(PORT_D, (PIN_6 | PIN_7));
		gpio_config_port_ctl(PORT_D, (PIN_6 | PIN_7), 1);
	#endif
}

void init_capsense_i2c(void)
{
	i2c_enable(I2C0_CGC | I2C1_CGC | I2C2_CGC | I2C3_CGC);
	i2c_init_master(I2C0);
	i2c_init_master(I2C1);
	i2c_init_master(I2C2);
	i2c_init_master(I2C3);
	i2c_config_speed(I2C0, I2C_TIMER_PERIOD_100K);
	i2c_config_speed(I2C1, I2C_TIMER_PERIOD_100K);
	i2c_config_speed(I2C2, I2C_TIMER_PERIOD_100K);
	i2c_config_speed(I2C3, I2C_TIMER_PERIOD_100K);
	//i2c_clock_low_timout_set(I2C0, I2C_CLOCK_LOW_TIMEOUT);
	//i2c_clock_low_timout_set(I2C1, I2C_CLOCK_LOW_TIMEOUT);
	//i2c_clock_low_timout_set(I2C2, I2C_CLOCK_LOW_TIMEOUT);
	//i2c_clock_low_timout_set(I2C3, I2C_CLOCK_LOW_TIMEOUT);
	i2c_master_interrupt_enable(I2C0, 4);
	i2c_master_interrupt_enable(I2C1, 4);
	i2c_master_interrupt_enable(I2C2, 4);
	i2c_master_interrupt_enable(I2C3, 4);
	//i2c_clock_timeout_interrupt_enable(I2C0, 4);
	//i2c_clock_timeout_interrupt_enable(I2C1, 4);
	//i2c_clock_timeout_interrupt_enable(I2C2, 4);
	//i2c_clock_timeout_interrupt_enable(I2C3, 4);
}

void init_capsense_buffers(void)
{
	touch_read_buffer = touch_buffers[0];
	touch_write_buffer = touch_buffers[1];
}

void init_capsense_uart(void)
{
	#ifndef USING_SIMULATOR
		uart_clock_enable(UART2_CGC);
		uart_channel_disable(UART2);
		uart_config_baud(UART2, 460800);
		uart_config_line_control(UART2, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
		uart_config_dma(UART2, UART_DMACTL_RX_EN | UART_DMACTL_TX_EN);
		uart_enable_interrupts(UART2, 1);
		uart_channel_enable(UART2, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
		UART2->IFLS = 0;
		UART2->IFLS |= 0x13;
	#endif
}

void queue_touch_buffer_dma(void)
{
	DMA_control touch_buffer_tx_req;

	touch_buffer_tx_req.source = (void*) (((uint32_t) &(touch_read_buffer[TOUCH_BUFFER_SIZE])) - 1);
	#ifdef USING_SIMULATOR
		touch_buffer_tx_req.destination = (void*) &(UART0->DR);
	#else
		// THIS IS ALSO RIGHT FUCK CARTER U RULE
		touch_buffer_tx_req.destination = (void*) &(UART2->DR);
	#endif
	touch_buffer_tx_req.control = (DMA_DSTINC_NONE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_2 | (TOUCH_BUFFER_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
		
	dma_primary_control_structure_set(CAPSENSE_OUTPUT_DMA_CHANNEL, &touch_buffer_tx_req);	
		
	dma_channel_enable(CAPSENSE_OUTPUT_DMA_CHANNEL);
}

void init_capsense_dma(void)
{
	dma_enable();
	
	dma_priority_set(CAPSENSE_OUTPUT_DMA_CHANNEL, DMA_PRIORITY_HIGH);
	dma_use_primary_control(CAPSENSE_OUTPUT_DMA_CHANNEL);
	dma_use_burst(CAPSENSE_OUTPUT_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
	dma_peripheral_request_mask_set(CAPSENSE_OUTPUT_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
	
	#ifdef USING_SIMULATOR
		UDMA->CHMAP1 &= ~(0xF0);
	#else
		// THIS SEEMS RIGHT FOR NOW
		UDMA->CHMAP0 &= ~(0xF0);
		UDMA->CHMAP0 |= (0x10);
	#endif
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
	uint32_t *temp;

	touch_write_buffer[0] = 0;
	touch_write_buffer[1] = 0;
	touch_write_buffer[2] = 0;
	touch_write_buffer[3] = 0;
	
	// Section 1 or 3
	touch_write_buffer[0] |= (touch_sections[0].buffers[0] << 16);
	touch_write_buffer[1] |= (touch_sections[0].buffers[1] << 16);
	
	// Section 2 or 4
	touch_write_buffer[2] |= (touch_sections[1].buffers[0] << 16);
	touch_write_buffer[3] |= (touch_sections[1].buffers[1] << 16);
	
	// Section 5 or 7
	touch_write_buffer[0] |= touch_sections[2].buffers[0];
	touch_write_buffer[1] |= touch_sections[2].buffers[1];
	
	// Section 6 or 8
	touch_write_buffer[2] |= touch_sections[3].buffers[0];
	touch_write_buffer[3] |= touch_sections[3].buffers[1];
	
	temp = touch_read_buffer;
	touch_read_buffer = touch_write_buffer;
	touch_write_buffer = temp;
	
	if(!pushing_uart) {
		pushing_uart = true;
		queue_touch_buffer_dma();
	}
}

void capsense_write(Touch_section *section)
{
	i2c_slave_address_set(section->section_channel->channel, section->mappings[section->current_device].device_address);
	i2c_slave_rw_set(section->section_channel->channel, I2C_MSA_SLAVE_WRITE);
	section->section_channel->channel->MDR = CAPSENSE_PROX_REG_ADDR;
	section->section_channel->read_req = false;
	section->section_channel->channel->MCS = I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP;
}

void capsense_read(Touch_section *section)
{
	i2c_slave_address_set(section->section_channel->channel, section->mappings[section->current_device].device_address);
	i2c_slave_rw_set(section->section_channel->channel, I2C_MSA_SLAVE_READ);
	section->section_channel->read_req = true;
	section->section_channel->channel->MCS = I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP;
}

void process_capsense_section(Touch_section *section)
{
	uint8_t data, pixel;

	if(section->complete || !section->section_channel->update_pending)
		return;
		
	if(section->section_channel->status & (I2C_MCS_ARB_LOST | I2C_MCS_DATA_ACK | I2C_MCS_ADDR_ACK | I2C_MCS_ERROR)) {
		
		if(section->section_channel->attempt < MAX_TRIES_I2C) {
			section->section_channel->attempt++;
			
			if(section->section_channel->read_req)
				capsense_read(section);
			else
				capsense_write(section);
		} else { // Failed request
			if (section->section_channel == &i2c0) {
				section->section_channel->attempt = 0;
			}
		
			section->section_channel->attempt = 0;
			section->current_device++;
			
			while(section->mappings[section->current_device].device_address == 0x00
				&& section->current_device < CAPSENSE_CONTROLLERS_PER_SECTION)
				section->current_device++;
			
			if(section->current_device >= CAPSENSE_CONTROLLERS_PER_SECTION) {
				section->complete = true;
			} else {
				capsense_write(section);
			}
		}
	} else {
		if(section->section_channel->read_req) {
			// Save off the data
			data = section->section_channel->channel->MDR;
			
			if((data & BIT_0) == BIT_0) {
				pixel = section->mappings[section->current_device].bit_0_pixel;
				section->buffers[pixel / 16] |= (0x01 << (15 - (pixel % 16)));
			}
			
			if((data & BIT_1) == BIT_1) {
				pixel = section->mappings[section->current_device].bit_1_pixel;
				section->buffers[pixel / 16] |= (0x01 << (15 - (pixel % 16)));
			}
			
			// Start the next section
			section->current_device++;
			
			while(section->mappings[section->current_device].device_address == 0x00
				&& section->current_device < CAPSENSE_CONTROLLERS_PER_SECTION)
				section->current_device++;
			
			if(section->current_device >= CAPSENSE_CONTROLLERS_PER_SECTION) {
				section->complete = true;
			} else {
				capsense_write(section);
			}
		} else {
			capsense_read(section);
		}
	}
	
	section->section_channel->update_pending = false;
}

void reset_touch_sections(void)
{
	int i = 0;
	for(; i < ACTIVE_SECTIONS_CAPSENSE; i++) {
		touch_sections[i].complete = false;
		touch_sections[i].buffers[0] = 0;
		touch_sections[i].buffers[1] = 0;
		touch_sections[i].current_device = 0;
		touch_sections[i].section_channel->update_pending = false;
		touch_sections[i].section_channel->attempt = 0;
		capsense_write(&touch_sections[i]);
	}
}

void poll_capsense(void)
{
	int i;
	polling_capsense = true;
	reset_touch_sections();
	
	while(!all_sections_complete()) {
		for(i = 0; i < ACTIVE_SECTIONS_CAPSENSE; i++) {
			process_capsense_section(&touch_sections[i]);
		}
	}
	
	transmit_touch_buffer();
	capsense_poll_requested = false;
	polling_capsense = false;
}


void init_capsense(void)
{	
	int i;

	init_capsense_gpio();
	init_capsense_uart();
	init_capsense_i2c();
	init_capsense_buffers();
	init_capsense_dma();
	poll_capsense();
	give_lock();
}