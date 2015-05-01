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

// CR0 values
#define SPI_FRF_FREESCALE		0x00
#define SPI_FRF_TI				0x01
#define SPI_FRF_MICROWIRE		0x02
#define SPI_PHASE_FIRST_CLK		0x00
#define SPI_PHASE_SECOND_CLK	0x01
#define SPI_POLARITY_LOW		0x00
#define SPI_POLARITY_HIGH		0x01

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

// DMACTL values
#define SPI_DMACTL_TX_EN		0x02
#define SPI_DMACTL_RX_EN		0x01

void spi_clock_enable(uint8_t spis);
void spi_disable(SSI0_Type *spi);
void spi_enable(SSI0_Type *spi);
void spi_set_master_slave(SSI0_Type *spi, uint8_t mode);
void spi_set_clock_source(SSI0_Type *spi, uint8_t source);
void spi_set_clock_speed(SSI0_Type *spi, uint8_t source);
void spi_set_frame_format(SSI0_Type *spi, uint8_t frame_format);
void spi_set_clock_phase(SSI0_Type *spi, uint8_t phase);
void spi_set_clock_polarity(SSI0_Type *spi, uint8_t polarity);
void spi_set_data_size(SSI0_Type *spi, uint8_t data_size);
void spi_config_dma(SSI0_Type *spi, uint8_t dma_config);
void spi_enable_interrupts(SSI0_Type *spi, uint8_t priority);

#endif