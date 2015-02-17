#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

// C
#include <stdint.h>

// MCU
#include "TM4C123.h"

// Project
#include "pc_buffer.h"
#include "uart.h"

#define UART_MIS_TXMIS          0x00000020  // UART Transmit Masked Interrupt
                                            // Status

void DisableInterrupts(void);
void EnableInterrupts(void);

#endif
