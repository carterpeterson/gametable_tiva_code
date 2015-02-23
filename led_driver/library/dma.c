// Project
#include "dma.h"

__align(1024) DMA_control dma_control_structure[64];

bool dma_enable(void)
{
	SYSCTL->RCGCDMA = 0x1;
	
	while((SYSCTL->PRDMA & 0x01) != 0x01)
		; // Wait
	
	UDMA->CFG = 0x01;
	UDMA->CTLBASE = (uint32_t) &dma_control_structure;
	
	return true;
}

bool dma_channel_enable(uint8_t channel)
{
	if(channel > 31)
		return false;

	UDMA->ENASET |= (0x01 << channel);
	
	return true;
}

bool dma_channel_disable(uint8_t channel)
{
	if(channel > 31)
		return false;

	UDMA->ENACLR |= (0x01 << channel);
	
	return true;
}

bool dma_channel_request(uint8_t channel)
{
	if(channel > 31)
		return false;

	UDMA->SWREQ |= (0x01 << channel);
	
	return true;
}

bool dma_primary_control_structure_set(uint8_t channel, DMA_control *control)
{
	if(channel > 31)
		return false;
	
	dma_control_structure[channel].destination = control->destination;
	dma_control_structure[channel].source = control->source;
	dma_control_structure[channel].control = control->control;
	
	return true;
}

bool dma_secondary_control_structure_set(uint8_t channel, DMA_control *control)
{
	if(channel > 31)
		return false;
	
	dma_control_structure[channel + 32].destination = control->destination;
	dma_control_structure[channel + 32].source = control->source;
	dma_control_structure[channel + 32].control = control->control;
	
	return true;
}

bool dma_priority_set(uint8_t channel, uint8_t priority)
{
	if(channel > 31)
		return false;
	
	switch(priority) {
	case DMA_PRIORITY_DEFAULT:
		UDMA->PRIOCLR |= (0x01 << channel);
		break;
	case DMA_PRIORITY_HIGH:
		UDMA->PRIOSET |= (0x01 << channel);
		break;
	default:
		// Do nothing
		break;
	}
	
	return true;
}

bool dma_use_alternate_control(uint8_t channel)
{
	if(channel > 31)
		return false;
	
	UDMA->ALTSET |= (0x01 << channel);
	
	return true;
}

bool dma_use_primary_control(uint8_t channel)
{
	if(channel > 31)
		return false;
	
	UDMA->ALTCLR |= (0x01 << channel);
	
	return true;
}

bool dma_use_burst(uint8_t channel, uint8_t use_burst)
{
	if(channel > 31)
		return false;
	
	switch(use_burst) {
	case DMA_USE_BURST_AND_SINGLE:
		UDMA->USEBURSTCLR |= (0x01 << channel);
		break;
	case DMA_USE_BURST_ONLY:
		UDMA->USEBURSTSET |= (0x01 << channel);
		break;
	default:
		// Do nothing
		break;
	}
	
	return true;
}

bool dma_peripheral_request_mask_set(uint8_t channel, uint8_t mask)
{
	if(channel > 31)
		return false;
	
	switch(mask){
	case DMA_REQ_MASK_PERIPHERAL:
		UDMA->REQMASKCLR |= (0x01 << channel);
		break;
	case DMA_REQ_MASK_SOFTWARE_ONLY:
		UDMA->REQMASKSET |= (0x01 << channel);
		break;
	default:
		// Do nothing
		break;
	}
	
	return 0;
}
