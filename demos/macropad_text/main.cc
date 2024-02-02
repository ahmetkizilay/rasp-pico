#include <stdio.h>

#include "adafruit_macropad/adafruit_macropad.h"
#include "display_manager/display_manager.h"
#include "font/dotmatrix_font.h"
#include "hardware/spi.h"
#include "pico/time.h"
#include "sh1106_spi/sh1106_spi.h"

using ::crynsnd::DisplayManager;
using ::crynsnd::SH1106;
using ::crynsnd::fonts::DotMatrix;

int main() {
  SH1106 sh1106(spi1, MACROPAD_DISPLAY_MISO, MACROPAD_DISPLAY_MOSI,
                MACROPAD_DISPLAY_SCK, MACROPAD_DISPLAY_CS,
                MACROPAD_DISPLAY_RESET, MACROPAD_DISPLAY_DC);
  sh1106.init();

  DotMatrix font;

  DisplayManager display(&sh1106, &font);
  display.clear();

  display.write("HELLO WORLD! LOREM IPSUM DOLOR SIT AMET CONSECTETUR");

  while (true) {
    sleep_ms(100);
  }
}