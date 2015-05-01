#include "spi.h"

void spi_clock_enable(uint8_t spis)
{
	SYSCTL->RCGCSSI |= spis;
	
	while((SYSCTL->PRSSI & spis) != spis)
		; // Wait for SPIs to be ready
}

void spi_disable(SSI0_Type *spi)
{
	spi->CR1 = 0;
}

void spi_enable(SSI0_Type *spi)
{
	spi->CR1 |= SPI_ENABLE;
}

void spi_set_mode(SSI0_Type *spi, uint8_t mode)
{
	spi->CR1 |= mode;
}

void spi_set_clock_source(SSI0_Type *spi, uint8_t source)
{
	spi->CC = source;
}

// THIS FUNCTION ASSUMES 80MHZ CLOCK!!!
void spi_set_clock_speed(SSI0_Type *spi, uint8_t source)
{
	switch(source) {
	case SPI_CLOCK_SPEED_20_MHZ:
		spi->
	break;
	
	}
}