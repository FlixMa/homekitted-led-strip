#ifndef _HOMEKIT_CONFIG_H_
#define _HOMEKIT_CONFIG_H_

#include <homekit/homekit.h>

void init_homekit();
void broadcast_change();

void identify_strip(homekit_value_t _value);

homekit_value_t is_strip_turned_on();
void set_strip_on(homekit_value_t value);

homekit_value_t get_strip_hue();
void set_strip_hue(homekit_value_t value);

homekit_value_t get_strip_saturation();
void set_strip_saturation(homekit_value_t value);

homekit_value_t get_strip_brightness();
void set_strip_brightness(homekit_value_t value);

#endif
