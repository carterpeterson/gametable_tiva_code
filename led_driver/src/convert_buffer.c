#include "convert_buffer.h"

void convert_buffer(void)
{
	int j, i;
	for(i = 0; i < 2; i++) {
		for( j = 0; j < 8; j++) {
			if(((read_buffer[i].green << j) & 0x0080) == 0x0080) {
				frame_buffer[(i*24) + j] = 1;
			} else {
				frame_buffer[(i*24) + j] = 0;
			}
			
			if(((read_buffer[i].red << j) & 0x0080) == 0x0080) {
				frame_buffer[(i*24) + j + 8] = 1;
			} else {
				frame_buffer[(i*24) + j + 8] = 0;
			}
			
			if(((read_buffer[i].blue << j) & 0x0080) == 0x0080) {
				frame_buffer[(i*24) + j + 16] = 1;
			} else {
				frame_buffer[(i*24) + j + 16] = 0;
			}
		}
	}
	
	switch_buffers();
}
