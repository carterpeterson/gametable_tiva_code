#include "interrupt_routines.h"

void UART0_Handler(void)
{    
	uint32_t  status;
    status = UDMA->CHIS;
	
	if((status & (0x01 << 8)) == (0x01 << 8)){		
		UDMA->CHIS |= (0x01 << 8); // Clear
	}
	
	pushing_frame = false;
}
