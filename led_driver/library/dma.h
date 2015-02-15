#ifndef __DMA_H__
#define __DMA_H__

// C
#include <stdint.h>
#include <stdbool.h>

// MCU
#include "TM4C123.h"

typedef struct {
	void *source;
	void *destination;
	uint32_t control;
	uint32_t unused;
} DMA_control;

extern DMA_control dma_control_structure[64];

bool dma_enable(void);
void dma_test(void);

#endif
