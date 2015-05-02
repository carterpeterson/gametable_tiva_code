#ifndef __LCD_MENU_H__
#define __LCD_MENU_H__

// C
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"
#include "system_TM4C123.h"

// Project
#include "app_config.h"
#include "../drivers/gpio.h"
#include "../drivers/spi.h"
#include "../drivers/dma.h"
#include "hypervisor.h"
#include "lcd_graphics.h"
#include "tasks.h"

#define LCD_SPI			SSI0
#define LCD_PORT		PORT_A
#define LCD_CLOCK_PIN	PIN_2
#define LCD_CS_PIN		PIN_3
#define LCD_CD_PIN		PIN_4
#define LCD_MOSI_PIN	PIN_5
#define LCD_RESET_PIN 	PIN_3

#define LCD_DMA_CHANNEL	11

#define LCD_ROW_SIZE_MINUS_ONE	101
#define LCD_ROW_SIZE			102

extern bool lcd_pushing_menu, lcd_pushing_init;
extern uint8_t lcd_current_page;
extern uint8_t *lcd_buffer_read, *lcd_buffer_write;

void init_lcd_menu(void);
void lcd_set_command_mode(void);
void lcd_set_data_mode(void);
void lcd_set_page(uint8_t page);
void lcd_set_column(uint8_t column);
void lcd_write_string_8pts( uint8_t line, char *string);
void lcd_update(void);

#endif