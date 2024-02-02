#include <stdio.h>

#include "adafruit_macropad/adafruit_macropad.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "sh1106_spi/sh1106_spi.h"

using ::crynsnd::SH1106;

void drawOnePixelAtATime(SH1106& sh1106) {
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 128; x++) {
        sh1106.setPixel(x, y);
        sh1106.flush();
        sleep_ms(10);
      }
    }
    sh1106.clearDisplay();
    sh1106.flush();
}

int main() {
  stdio_init_all();
  SH1106 sh1106(spi1, MACROPAD_DISPLAY_MISO, MACROPAD_DISPLAY_MOSI,
                MACROPAD_DISPLAY_SCK, MACROPAD_DISPLAY_CS,
                MACROPAD_DISPLAY_RESET, MACROPAD_DISPLAY_DC);
  sh1106.init();
  sh1106.clearDisplay();
  sh1106.flush();

  while (true) {
    drawOnePixelAtATime(sh1106);
  }
}