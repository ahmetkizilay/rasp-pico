#include <stdio.h>

#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "ws2812_pio/ws2812_pio.h"

#define MACROPAD_LED 13
#define MACROPAD_NEOPIXEL 19

using ::crynsnd::WS2812;

static inline uint32_t make_color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

int main() {
  stdio_init_all();

  gpio_init(MACROPAD_LED);
  gpio_set_dir(MACROPAD_LED, GPIO_OUT);

  // RP2040 has 2 PIOs with 4 state machines.
  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);

  WS2812 ws2012 = WS2812::Init(pio, sm, MACROPAD_NEOPIXEL);
  while (true) {
    gpio_put(MACROPAD_LED, 1);
    sleep_ms(500);

    ws2012.put_blocking(make_color(0xff, 0, 0) << 8u);
    ws2012.put_blocking(make_color(0, 0xff, 0) << 8u);
    ws2012.put_blocking(make_color(0, 0, 0xff) << 8u);

    ws2012.put_blocking(make_color(0xff, 0, 0) << 8u);
    ws2012.put_blocking(make_color(0, 0xff, 0) << 8u);
    ws2012.put_blocking(make_color(0, 0, 0xff) << 8u);

    gpio_put(MACROPAD_LED, 0);
    sleep_ms(500);
  }
}