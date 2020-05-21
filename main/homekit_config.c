#include <stdio.h>
#include <homekit/characteristics.h>

#include "homekit_config.h"
#include "leds.h"

homekit_characteristic_t strip_is_on_characteristic = HOMEKIT_CHARACTERISTIC_(
    ON, false,
    .getter = is_strip_turned_on,
    .setter = set_strip_on
);
homekit_characteristic_t strip_hue_characteristic = HOMEKIT_CHARACTERISTIC_(
    HUE, 0,
    .getter = get_strip_hue,
    .setter = set_strip_hue
);
homekit_characteristic_t strip_saturation_characteristic = HOMEKIT_CHARACTERISTIC_(
    SATURATION, 0,
    .getter = get_strip_saturation,
    .setter = set_strip_saturation
);
homekit_characteristic_t strip_brightness_characteristic = HOMEKIT_CHARACTERISTIC_(
    BRIGHTNESS, 100,
    .getter = get_strip_brightness,
    .setter = set_strip_brightness
);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_lightbulb, .services = (homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "LED Streifen"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "ESPRESSIF"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "FMA-2020-05-21"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP32-WROOM-32"),
            //HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, identify_strip),
            NULL
        }),
        HOMEKIT_SERVICE(LIGHTBULB, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
            //HOMEKIT_CHARACTERISTIC(NAME, "Sample LED Strip"),
            &strip_is_on_characteristic,
            &strip_hue_characteristic,
            &strip_saturation_characteristic,
            &strip_brightness_characteristic,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t homekit_config = {
    .accessories = accessories,
    .password = "111-11-111"
};


void init_homekit() {
   homekit_server_init(&homekit_config);
}

void broadcast_change() {
    homekit_characteristic_notify(&strip_is_on_characteristic, is_strip_turned_on());
    homekit_characteristic_notify(&strip_hue_characteristic, get_strip_hue());
    homekit_characteristic_notify(&strip_saturation_characteristic, get_strip_saturation());
    homekit_characteristic_notify(&strip_brightness_characteristic, get_strip_brightness());
}


void identify_strip(homekit_value_t _value) {
    printf("identify myself :-)");
    strip_identify();
}


homekit_value_t is_strip_turned_on() {
    return HOMEKIT_BOOL(is_strip_on);
}
void set_strip_on(homekit_value_t value) {
    if (value.format != homekit_format_bool) {
        // printf("Invalid on-value format: %d\n", value.format);
        return;
    }

    if (is_strip_on != value.bool_value) {
        strip_brightness = strip_brightness > CONFIG_MINIMUM_BRIGHTNESS_ON_TURN_ON ? strip_brightness : CONFIG_MINIMUM_BRIGHTNESS_ON_TURN_ON;
    }
    is_strip_on = value.bool_value;
    strip_update();
}


homekit_value_t get_strip_hue() {
    return HOMEKIT_FLOAT(strip_hue);
}
void set_strip_hue(homekit_value_t value) {
    if (value.format != homekit_format_float) {
        // printf("Invalid hue-value format: %d\n", value.format);
        return;
    }
    strip_hue = value.float_value;
    strip_update();
}


homekit_value_t get_strip_saturation() {
    return HOMEKIT_FLOAT(strip_saturation);
}
void set_strip_saturation(homekit_value_t value) {
    if (value.format != homekit_format_float) {
        // printf("Invalid sat-value format: %d\n", value.format);
        return;
    }
    strip_saturation = value.float_value;
    strip_update();
}


homekit_value_t get_strip_brightness() {
    return HOMEKIT_INT(strip_brightness);
}

void set_strip_brightness(homekit_value_t value) {
    if (value.format != homekit_format_int) {
        // printf("Invalid brightness-value format: %d\n", value.format);
        return;
    }
    strip_brightness = value.int_value;
    strip_update();
}
