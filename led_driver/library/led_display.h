#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__

typedef struct {
	uint8_t red, green, blue;
} Pixel;

extern void pushBuffer(Pixel *i);

#endif
