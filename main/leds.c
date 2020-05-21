#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/rmt.h>

#include "leds.h"

#define RMT_TX_CHANNEL RMT_CHANNEL_0
static const char *TAG = "example";

led_strip_t *strip = 0;

void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}

void strip_init(void) {

    rmt_config_t config = {                           \
        .rmt_mode = RMT_MODE_TX,                      \
        .channel = RMT_TX_CHANNEL,                    \
        .gpio_num = CONFIG_GPIO_STRIP_DATA_PIN,       \
        .clk_div = 2, /* set counter clock to 40MHz*/ \
        .mem_block_num = 1,                           \
        .tx_config = {                                \
            .carrier_freq_hz = 38000,                 \
            .carrier_level = RMT_CARRIER_LEVEL_HIGH,  \
            .idle_level = RMT_IDLE_LEVEL_LOW,         \
            .carrier_duty_percent = 33,               \
            .carrier_en = false,                      \
            .loop_en = false,                         \
            .idle_output_en = true,                   \
        }                                             \
    };


    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_STRIP_TOTAL_NUM_LEDS, (led_strip_dev_t)config.channel);
    strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100));
}

void strip_update(void) {
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    led_strip_hsv2rgb(strip_hue, strip_saturation, is_strip_on ? strip_brightness : 0, &red, &green, &blue);

    for (int i = 0; i < CONFIG_STRIP_TOTAL_NUM_LEDS; ++i) {
        ESP_ERROR_CHECK(strip->set_pixel(strip, i, red, green, blue));
    }
    ESP_ERROR_CHECK(strip->refresh(strip, 100));
}

void strip_identify(void) {

    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    uint16_t hue = 0;

    // Show simple rainbow chasing pattern
    ESP_LOGI(TAG, "LED Rainbow Chase Start");
    for (uint16_t start_rgb = 0; start_rgb < 1000; start_rgb += 60) {
        for (int i = 0; i < 3; i++) {
            for (int j = i; j < CONFIG_STRIP_TOTAL_NUM_LEDS; j += 3) {
                // Build RGB values
                hue = j * 360 / CONFIG_STRIP_TOTAL_NUM_LEDS + start_rgb;
                led_strip_hsv2rgb(hue, 100, 100, &red, &green, &blue);
                // Write RGB values to strip driver
                ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
            }
            // Flush RGB values to LEDs
            ESP_ERROR_CHECK(strip->refresh(strip, 100));
            vTaskDelay(pdMS_TO_TICKS(10));
            strip->clear(strip, 50);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    strip_update();
}
