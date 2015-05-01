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

#define LCD_SPI			SSI0
#define LCD_PORT		PORT_A
#define LCD_CLOCK_PIN	PIN_2
#define LCD_CS_PIN		PIN_3
#define LCD_CD_PIN		PIN_4
#define LCD_MOSI_PIN	PIN_5
#define LCD_RESET_PIN 	PIN_3

#define LCD_DMA_CHANNEL	11

extern bool pushing_lcd_menu;

void init_lcd_menu(void);
void set_lcd_command_mode(void);
void set_lcd_data_mode(void);

#endif