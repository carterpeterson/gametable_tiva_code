// Project
#include "dma.h"

__align(1024) DMA_control dma_control_structure[64];

	uint8_t source[16];
	uint8_t dest[16];

bool dma_enable(void)
{
	SYSCTL->RCGCDMA = 0x1;
	
	while((SYSCTL->PRDMA & 0x01) != 0x01)
		; // Wait
	
	UDMA->CFG = 0x01;
	UDMA->CTLBASE = (uint32_t) &dma_control_structure;
	
	return true;
}

bool dma_set_primary_control_structure(uint8_t channel, DMA_control *control)
{
	if(channel > 31)
		return false;
	
	dma_control_structure[channel].destination = control->destination;
	dma_control_structure[channel].source = control->source;
	dma_control_structure[channel].control = control->control;
	
	return true;
}

bool dma_set_secondary_control_structure(uint8_t channel, DMA_control *control)
{
	if(channel > 31)
		return false;
	
	dma_control_structure[channel + 32].destination = control->destination;
	dma_control_structure[channel + 32].source = control->source;
	dma_control_structure[channel + 32].control = control->control;
	
	return true;
}

bool dma_set_priority(uint8_t channel, uint8_t priority)
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

void dma_test(void)
{
	int i = 0;
	
	source[0] = 't';
	source[1] = 'e';
	source[2] = 's';
	source[3] = 't';
	source[4] = '\n';
	source[5] = '\r';
	
	dma_set_priority(30, DMA_PRIORITY_DEFAULT);
	dma_use_primary_control(30);
	dma_use_burst(30, DMA_USE_BURST_AND_SINGLE);
	
	UDMA->REQMASKCLR  |= (0x01 << 30);
	
	dma_control_structure[30].source = &source[5];
	dma_control_structure[30].destination = (void*) &(UART0->DR);//&dest[16];
	dma_control_structure[30].control = (DMA_DSTINC_NONE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_1 | (6U << 4) | DMA_XFERMODE_AUTO_REQUEST);
	
	UDMA->ENASET |= (0x01 << 30);
	UDMA->SWREQ |= (0x01 << 30);
	
	for(; i < 1000; i++)
		; // Wait
	
	printf("done\n");
}
