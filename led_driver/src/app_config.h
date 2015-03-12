#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "../library/led_display.h"

#define STRING_LENGTH_COLOR		15
#define STRING_LENGTH_BITS		120

#define INPUT_BUFF_MINUS_ONE	14U

extern Pixel *read_buffer;
extern Pixel *write_buffer;

#endif
