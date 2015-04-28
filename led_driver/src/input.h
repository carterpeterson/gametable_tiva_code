#ifndef __INPUT_H__
#define __INPUT_H__

// C

// MCU

// Project
#include "../library/dma.h"
#include "../library/uart.h"
#include "led_display.h"
#include "app_config.h"

extern Pixel *read_buffer;
extern Pixel *write_buffer;

extern bool refresh_screen_req;

void init_input(void);

#endif
