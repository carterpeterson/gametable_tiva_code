#include "lcd_menu.h"

void init_lcd_menu_gpio(void)
{
	// Setup the GPIO for SPI bus
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MISO_PIN | LCD_MOSI_PIN));
	gpio_pin_direction(LCD_PORT, DIRECTION_OUTPUT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MISO_PIN | LCD_MOSI_PIN));
	gpio_alternate_function_enable(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MISO_PIN | LCD_MOSI_PIN));
	gpio_config_port_ctl(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MISO_PIN | LCD_MOSI_PIN), 2);
	
	// Enable the reset pin and bring the LCD out of reset
	gpio_port_enable(PORT_B_CGC);
	gpio_digital_enable(PORT_B, LCD_RESET_PIN);
	gpio_pin_direction(PORT_B, DIRECTION_OUTPUT, LCD_RESET_PIN);
	PORT_B->DATA |= LCD_RESET_PIN;
}

void init_lcd_menu_spi(void)
{
	spi_clock_enable(SPI0_CGC);
	
}

void init_lcd_menu(void)
{
	init_lcd_menu_gpio();
	init_lcd_menu_spi();
}