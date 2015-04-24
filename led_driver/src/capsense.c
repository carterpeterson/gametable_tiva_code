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
	{{0x24, 12, 13}, {0x25, 16, 17}, {0x22, 2, 3}, {0x23, 18, 19},
		{0}, {0}, {0}, {0},
		{0}, {0}, {0}, {0},
		{0}, {0}, {0}, {0}}}
};

uint32_t touch_buffers[2][TOUCH_BUFFER_SIZE] = {0};
uint32_t *touch_read_buffer, *touch_write_buffer;
bool pushing_uart = false;

void init_capsense_gpio(void)
{
	// I2C1
	gpio_digital_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_alternate_function_enable(PORT_A, (PIN_6 | PIN_7));
	gpio_config_port_ctl(PORT_A, (PIN_6 | PIN_7), 3);
	gpio_open_drain_enable(PORT_A, PIN_7);
	
	#ifndef USING_SIMULATOR
		// UART 7
		gpio_port_enable(PORT_E_CGC);
		gpio_digital_enable(PORT_E, (PIN_0 | PIN_1));
		gpio_pin_direction(PORT_E, DIRECTION_OUTPUT, PIN_1);
		gpio_pin_direction(PORT_E, DIRECTION_INPUT, PIN_0);
		gpio_alternate_function_enable(PORT_E, (PIN_0 | PIN_1));
		gpio_config_port_ctl(PORT_E, (PIN_0 | PIN_1), 1);
	#endif
}

void init_capsense_i2c(void)
{
	i2c_enable(I2C1_CGC);
	i2c_config_speed(I2C1, I2C_TIMER_PERIOD_400K);
	i2c_init_master(I2C1);
	i2c_master_interrupt_enable(I2C1, 1);
}

void init_capsense_buffers(void)
{
	touch_read_buffer = touch_buffers[0];
	touch_write_buffer = touch_buffers[1];
}

void init_capsense_uart(void)
{
	#ifndef USING_SIMULATOR
		uart_clock_enable(UART7_CGC);
		uart_channel_disable(UART7);
		uart_config_baud(UART7, 460800);
		uart_config_line_control(UART7, (UART_CTL_WORD_LENGTH_8 | UART_LCRH_FEN));
		uart_config_dma(UART7, UART_DMACTL_RX_EN | UART_DMACTL_TX_EN);
		uart_enable_interrupts(UART7, 1);
		uart_channel_enable(UART7, UART_CTL_ENABLE | UART_CTL_RX_ENABLE | UART_CTL_TX_ENABLE);
		UART7->IFLS = 0;
		UART7->IFLS |= 0x13;
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
		touch_buffer_tx_req.destination = (void*) &(UART7->DR);
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
		UDMA->CHMAP2 &= ~(0x00F00000);
		UDMA->CHMAP2 |= (0x00200000);
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
	int i;
	uint32_t *temp;

	touch_write_buffer[0] = 0;
	touch_write_buffer[1] = 0;
	
	touch_write_buffer[0] |= (touch_sections[0].buffers[0] << 16);
	touch_write_buffer[1] |= (touch_sections[0].buffers[1] << 16);
	
	temp = touch_read_buffer;
	touch_read_buffer = touch_write_buffer;
	touch_write_buffer = temp;
	
	if(!pushing_uart) {
		pushing_uart = true;
		for(i = 0; i < 250000; i++) {
			// wait
		}
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

void init_capsense(void)
{	
	int i;

	init_capsense_gpio();
	init_capsense_uart();
	init_capsense_i2c();
	init_capsense_buffers();
	init_capsense_dma();
	reset_touch_sections();
	
	while(1) {
		if(all_sections_complete()) {
			transmit_touch_buffer();
			reset_touch_sections();
		} else {
			for(i = 0; i < ACTIVE_SECTIONS_CAPSENSE; i++) {
				process_capsense_section(&touch_sections[i]);
			}
		}
	}
}
