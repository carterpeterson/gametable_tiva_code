// Project
#include "dma.h"

__align(1024) DMA_control dma_control_structure[64];

bool dma_enable(void)
{
	SYSCTL->RCGCDMA = 0x1;
	
	while((SYSCTL->PRDMA & 0x01) != 0x01)
		; // Wait
	
	return true;
}

void dma_test(void)
{
	UDMA->CFG = 0x01;
	UDMA->CTLBASE = (uint32_t) &dma_control_structure;
}
