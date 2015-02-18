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
	
	return true;
}

void dma_test(void)
{
	int i = 0;
	
	source[0] = 1;
	source[1] = 2;
	source[2] = 3;
	source[3] = 4;
	source[4] = 5;
	
	UDMA->CFG = 0x01;
	UDMA->CTLBASE = (uint32_t) &dma_control_structure;
	
	UDMA->PRIOSET |= (0x01 << 30);
	UDMA->ALTCLR |= (0x01 << 30);
	UDMA->USEBURSTCLR |= (0x01 << 30);
	UDMA->REQMASKCLR  |= (0x01 << 30);
	
	dma_control_structure[30].source = &source[16];
	dma_control_structure[30].destination = &dest[16];
	dma_control_structure[30].control = (DMA_DSTINC_BYTE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_1 | (16U << 4) | DMA_XFERMODE_BASIC);
	
	UDMA->ENASET |= (0x01 << 30);
	UDMA->SWREQ |= (0x01 << 30);
	
	for(; i < 1000; i++)
		; // Wait
	
	printf("dest[0]: %d\n", dest[0]);
}
