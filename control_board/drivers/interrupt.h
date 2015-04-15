#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

// C
#include <stdint.h>

// MCU
#include "TM4C123.h"

// Project
#include "pc_buffer.h"
#include "uart.h"
#include "dma.h"
#include "../src/led_display.h"

void DisableInterrupts(void);
void EnableInterrupts(void);

#endif
