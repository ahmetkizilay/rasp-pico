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
  std::unique_ptr<SH1106> sh1106 = std::make_unique<SH1106>(
      spi1, MACROPAD_DISPLAY_MISO, MACROPAD_DISPLAY_MOSI, MACROPAD_DISPLAY_SCK,
      MACROPAD_DISPLAY_CS, MACROPAD_DISPLAY_RESET, MACROPAD_DISPLAY_DC);
  sh1106->init();

  std::unique_ptr<DotMatrix> font = std::make_unique<DotMatrix>();

  DisplayManager display(std::move(sh1106), std::move(font));
  display.clear();

  display.write("HELLO WORLD! LOREM IPSUM DOLOR SIT AMET CONSECTETUR");

  while (true) {
    sleep_ms(100);
  }
}