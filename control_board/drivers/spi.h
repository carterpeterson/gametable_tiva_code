#ifndef __SPI_H__
#define __SPI_H__

// C
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"
#include "system_TM4C123.h"

// Clock gating values
#define SPI0_CGC      0x01
#define SPI1_CGC      0x02
#define SPI2_CGC      0x04  
#define SPI3_CGC      0x08  

// CR1 values (SPI modes)
#define SPI_MODE_MASTER		0x00
#define SPI_MODE_SLAVE_OE	0x04
#define SPI_MODE_SLAVE_OD	0x0C
#define SPI_ENABLE			0x02

// CC values (clock source)
#define SPI_CLK_SRC_SYS		0x00
#define SPI_CLK_SRC_PIOSC	0x05

// Clock speeds
#define SPI_CLOCK_SPEED_20_MHZ	0x01

void spi_clock_enable(uint8_t spis);

#endif