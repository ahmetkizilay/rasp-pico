#include <stdio.h>

#include "adafruit_macropad/adafruit_macropad.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "sh1106_spi/sh1106_spi.h"

using ::crynsnd::SH1106;

int main() {
  stdio_init_all();
  SH1106 sh1106(spi1, MACROPAD_DISPLAY_MISO, MACROPAD_DISPLAY_MOSI,
                MACROPAD_DISPLAY_SCK, MACROPAD_DISPLAY_CS,
                MACROPAD_DISPLAY_RESET, MACROPAD_DISPLAY_DC);
  sh1106.init();
  sh1106.clearDisplay();
  sh1106.flush();

  // Fill the entire screen one pixel at a time.
  int x = 0;
  int y = 0;
  while (true) {
    sh1106.setPixel(x, y);
    sh1106.flush();
    sleep_ms(10);
    x += 1;
    if (x == 128) {
      x = 0;
      y += 1;
      if (y == 64) {
        y = 0;

        sh1106.clearDisplay();
        sh1106.flush();
      }
    }
  }
}