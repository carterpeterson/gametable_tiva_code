#include "lcd_menu.h"

bool pushing_lcd_menu = false;
static uint8_t lcd_init_commands[] = 
	{0x40,
	0xA1,
	0xC0,
	0xA4,
	0xA6,
	0xA2,
	0x2F,
	0x27,
	0x81,
	0x10,
	0xFA,
	0x90,
	0xAF};	// This is just their example setup from the EADOGS datasheet

void init_lcd_menu_gpio(void)
{
	// Setup the GPIO for SPI bus
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_CD_PIN | LCD_MOSI_PIN));
	gpio_pin_direction(LCD_PORT, DIRECTION_OUTPUT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_CD_PIN | LCD_MOSI_PIN));
	gpio_alternate_function_enable(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MOSI_PIN));
	gpio_config_port_ctl(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MOSI_PIN), 2);
	
	// Enable the reset pin and bring the LCD out of reset
	gpio_port_enable(PORT_B_CGC);
	gpio_digital_enable(PORT_B, LCD_RESET_PIN);
	gpio_pin_direction(PORT_B, DIRECTION_OUTPUT, LCD_RESET_PIN);
	PORT_B->DATA |= LCD_RESET_PIN;
}

void init_lcd_menu_spi(void)
{
	spi_clock_enable(SPI0_CGC);
	spi_disable(LCD_SPI);
	spi_set_master_slave(LCD_SPI, SPI_MODE_MASTER);
	spi_set_clock_source(LCD_SPI, SPI_CLK_SRC_SYS);
	spi_set_clock_speed(LCD_SPI, SPI_CLOCK_SPEED_20_MHZ);
	spi_set_frame_format(LCD_SPI, SPI_FRF_FREESCALE);
	spi_set_clock_polarity(LCD_SPI, SPI_POLARITY_HIGH);
	spi_set_clock_phase(LCD_SPI, SPI_PHASE_FIRST_CLK);
	spi_set_data_size(LCD_SPI, 8);
	spi_config_dma(LCD_SPI, SPI_DMACTL_TX_EN);
	spi_enable_interrupts(LCD_SPI, 4);
	spi_enable(LCD_SPI);
}

void init_lcd_menu_dma(void)
{
	dma_enable();
	
	dma_priority_set(LCD_DMA_CHANNEL, DMA_PRIORITY_HIGH);
	dma_use_primary_control(LCD_DMA_CHANNEL);
	dma_use_burst(LCD_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
	dma_peripheral_request_mask_set(LCD_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
	UDMA->CHMAP1 &= ~(0xF000);
}

void init_lcd_menu_commands(void)
{
	DMA_control lcd_setup_req;
	set_lcd_command_mode();

	lcd_setup_req.source = (void*) &(lcd_init_commands[12]);
	lcd_setup_req.destination = (void*) &(LCD_SPI->DR);
	lcd_setup_req.control = (DMA_DSTINC_NONE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_1 | (12U << 4) | DMA_XFERMODE_BASIC);
		
	dma_primary_control_structure_set(LCD_DMA_CHANNEL, &lcd_setup_req);	
		
	dma_channel_enable(LCD_DMA_CHANNEL);
	
	set_lcd_data_mode();
}

void set_lcd_command_mode(void)
{
	LCD_PORT->DATA &= ~LCD_CD_PIN;
}

void set_lcd_data_mode(void)
{
	LCD_PORT->DATA |= LCD_CD_PIN;
}

void init_lcd_menu(void)
{
	init_lcd_menu_gpio();
	init_lcd_menu_spi();
	init_lcd_menu_dma();
	init_lcd_menu_commands();
}