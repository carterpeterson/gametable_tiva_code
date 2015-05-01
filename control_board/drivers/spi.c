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

void spi_set_master_slave(SSI0_Type *spi, uint8_t mode)
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
		spi->CPSR = 2;
		spi->CR0 &= ~(0xFF << 8);
		spi->CR0 |= (0x02 << 8);
	break;
	default:
		// Do nothing
	break;
	}
}

void spi_set_frame_format(SSI0_Type *spi, uint8_t frame_format)
{
	spi->CR0 &= ~(0x03 << 4);
	spi->CR0 |= frame_format;
}

void spi_set_clock_phase(SSI0_Type *spi, uint8_t phase)
{
	spi->CR0 &= ~(0x01 << 7);
	spi->CR0 |= (phase << 7);
}

void spi_set_clock_polarity(SSI0_Type *spi, uint8_t polarity)
{
	spi->CR0 &= ~(0x01 << 6);
	spi->CR0 |= (polarity << 6);
}

void spi_set_data_size(SSI0_Type *spi, uint8_t data_size)
{
	spi->CR0 &= ~(0x0F);
	spi->CR0 |= (data_size - 1);
}

void spi_config_dma(SSI0_Type *spi, uint8_t dma_config)
{
	spi->DMACTL |= dma_config;
}

void spi_enable_interrupts(SSI0_Type *spi, uint8_t priority)
{
	IRQn_Type interrupt_vector;
	
	switch((uint32_t) spi) {
	case(SSI0_BASE):
		interrupt_vector = SSI0_IRQn;
		break;
	case(SSI1_BASE):
		interrupt_vector = SSI1_IRQn;;
		break;
	case(SSI2_BASE):
		interrupt_vector = SSI2_IRQn;;
		break;
	case(SSI3_BASE):
		interrupt_vector = SSI3_IRQn;;
		break;
	default:
		return;
	}
	
	// Set the priority
	NVIC_SetPriority(interrupt_vector, priority);

	// Enable the NVIC
	NVIC_EnableIRQ(interrupt_vector);
}
