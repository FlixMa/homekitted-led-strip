#ifndef _LEDS_H_
#define _LEDS_H_

#include <led_strip.h>

extern led_strip_t *strip;

float strip_hue;              // hue is scaled 0 to 360
float strip_saturation;      // saturation is scaled 0 to 100
float strip_brightness;     // brightness is scaled 0 to 100
bool is_strip_on;

void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b);
void strip_init(void);
void strip_update(void);
void strip_identify(void);

#endif // _LEDS_H_
