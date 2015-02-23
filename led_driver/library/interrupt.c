#include "interrupt.h"

void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

void UART0_Handler(void)
{    
	uint32_t  status;
    status = UDMA->CHIS;
	
	if((status & (0x01 << 8)) == (0x01 << 8)){
		
		dma_channel_request(30);
		
		UDMA->CHIS |= (0x01 << 30); // Clear
	}
}
