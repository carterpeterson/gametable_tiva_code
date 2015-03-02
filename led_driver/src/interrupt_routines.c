#include "interrupt_routines.h"

void UART0_Handler(void)
{    
	uint32_t  status;
	Pixel *temp;
	DMA_control test_uart_rx_req;
    status = UDMA->CHIS;
	
	if((status & (0x01 << 8)) == (0x01 << 8)){		
		temp = write_buffer;
		write_buffer = read_buffer;
		read_buffer = temp;
		
		dma_channel_enable(8);
		test_uart_rx_req.source = (void*) &(UART0->DR);
		test_uart_rx_req.destination = &(write_buffer[STRING_LENGTH_PIXEL - 1].blue);
		test_uart_rx_req.control = (DMA_DSTINC_BYTE | \
		DMA_DSTSIZE_BYTE | DMA_SRCINC_NONE | DMA_SRCSIZE_BYTE | \
		DMA_ARBSIZE_4 | (INPUT_BUFF_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
	
		dma_primary_control_structure_set(8, &test_uart_rx_req);
		
		
		UDMA->CHIS |= (0x01 << 30); // Clear
		
		printf("done\n\r");
		
		convert_buffer();
	}
}

void I2C1_Handler(void)
{
	int i = 0;
	for(; i < 200; i++)
		; // Wait
	
	printf("%x\n\r", I2C1->MCS);
	printf("%x\n\r", i2c1.current_request->device_addr);
	
	/*if(I2C1->MCS & (I2C_MCS_ARB_LOST | I2C_MCS_DATA_ACK | I2C_MCS_ADDR_ACK | I2C_MCS_ERROR)) {
		
		if(i2c1.attempt < MAX_TRIES_CAPSENSE) {
			printf("r");
			i2c1.attempt++;
			i2c_retry_request(&i2c1);
		} else {
			printf("d\n");
			i2c1.busy = false;
			i2c1.current_request = 0x00;
			i2c1.attempt = 0;
		}
		
	} else {
		//printf("y\n");
		
		i2c1.busy = false;
	}*/
	
	I2C1->MICR = 0x01;	// Clear
}
