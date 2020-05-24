#ifndef _LEDS_H_
#define _LEDS_H_

#include <led_strip.h>

extern led_strip_t *strip;

float strip_hue;              // hue is scaled 0 to 360
float strip_saturation;      // saturation is scaled 0 to 100
float strip_brightness;     // brightness is scaled 0 to 100
bool is_strip_on;

void led_strip_hsv2rgb(uint16_t h, uint16_t s, uint16_t v, uint16_t *r, uint16_t *g, uint16_t *b);
void strip_init(void);
void strip_update(void);
void strip_identify(void);
void strip_animation(void);

void shuffle(uint16_t *input, uint16_t *output, size_t n);


#endif // _LEDS_H_
