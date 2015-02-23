#ifndef __DMA_H__
#define __DMA_H__

// C
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// MCU
#include "TM4C123.h"

// DMA related defines

/*
*	DMA Channel Priority, whether or not the channel will be high
*	priority or default priority
*/
#define DMA_PRIORITY_DEFAULT	0
#define DMA_PRIORITY_HIGH		1

/*
*	DMA Useburst, determines whether the DMA channel will respond
*	to single requests in addition to burst requests.
*/
#define DMA_USE_BURST_AND_SINGLE	0
#define DMA_USE_BURST_ONLY			1

/*
*		Destination address increment:
*			This field configures the destination address increment.
*
*			The address increment value must be equal or greater than
*			the value of the destination size (DSTSIZE).
*/
#define DMA_DSTINC_BYTE (0x0U << 	30) // Increment by 8-bit locations
#define DMA_DSTINC_HALF	(0x1U <<	30)	// Increment by 16-bit locations
#define DMA_DSTINC_WORD (0x2U <<	30)	// Increment by 32-bit locations
#define DMA_DSTINC_NONE (0x3U <<	30)	// Do not increment


/*
*		Destination data size:
*			This field configures the destination item data size.
*
*			DSTSIZE must be the same as SRCSIZE
*/
#define DMA_DSTSIZE_BYTE	(0x0 << 28) 
#define DMA_DSTSIZE_HALF	(0x1 << 28)
#define DMA_DSTSIZE_WORD	(0x2 << 28)

/*
*		Source Address Increment:
*			This field configures the source address increment.
*
*			The address increment value must be equal or greater than the value
*			of the source size (SRCSIZE).
*/
#define DMA_SRCINC_BYTE (0x0 << 26)		// Increment by 8-bit locations
#define DMA_SRCINC_HALF	(0x1 << 26)		// Increment by 16-bit locations
#define DMA_SRCINC_WORD (0x2 << 26)		// Increment by 32-bit locations
#define DMA_SRCINC_NONE (0x3 << 26)		// Do not increment

/*
*		Source Data Size:
*			This field configures the source item data size.
*
*			Note: DSTSIZE must be the same as SRCSIZE.
*/
#define DMA_SRCSIZE_BYTE (0x0 << 24)
#define DMA_SRCSIZE_HALF (0x1 << 24)
#define DMA_SRCSIZE_WORD (0x2 << 24)

/*
*		Arbitration Size
*			This field configures the number of transfers that can occur before the
*			µDMA controller re-arbitrates. The possible arbitration rate configurations
*			represent powers of 2 and are shown below.
*/
#define DMA_ARBSIZE_1			(0x0 << 16)
#define DMA_ARBSIZE_2			(0x1 << 16)
#define DMA_ARBSIZE_4			(0x2 << 16)
#define DMA_ARBSIZE_8			(0x3 << 16)
#define DMA_ARBSIZE_16		(0x4 << 16)
#define DMA_ARBSIZE_32		(0x5 << 16)
#define DMA_ARBSIZE_64		(0x6 << 16)
#define DMA_ARBSIZE_128		(0x7 << 16)
#define DMA_ARBSIZE_256		(0x8 << 16)
#define DMA_ARBSIZE_512		(0x9 << 16)
#define DMA_ARBSIZE_1024 	(0xA << 16)

/*
*		Transfer Size (minus 1)
*			This field configures the total number of items to transfer. The value of
*			this field is 1 less than the number to transfer (value 0 means transfer
*			1 item). The maximum value for this 10-bit field is 1023 which represents
*			a transfer size of 1024 items.
*			
*			The transfer size is the number of items, not the number of bytes. If the
*			data size is 32 bits, then this value is the number of 32-bit words to
*			transfer.
*			
*			The µDMA controller updates this field immediately prior to entering the
*			arbitration process, so it contains the number of outstanding items that
*			is necessary to complete the µDMA cycle.
*/


/*
*		µDMA Transfer Mode:
*			This field configures the operating mode of the µDMA cycle. Refer to
*			“Transfer Modes” on page 591 for a detailed explanation of transfer
*			modes.
*			
*			Because this register is in system RAM, it has no reset value. Therefore,
*			this field should be initialized to 0 before the channel is enabled.
*/
#define DMA_XFERMODE_STOP								0x0
#define DMA_XFERMODE_BASIC							0x1
#define DMA_XFERMODE_AUTO_REQUEST				0x2
#define DMA_XFERMODE_PING_PONG					0x3
#define DMA_XFERMODE_MEM_SCT_GTH				0x4
#define DMA_XFERMODE_ALT_MEM_SCT_GTH		0x5
#define DMA_XFERMODE_PERIPH_SCT_GTH			0x6
#define DMA_XFERMODE_ALT_PERIPH_SCT_GTH	0x7

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
