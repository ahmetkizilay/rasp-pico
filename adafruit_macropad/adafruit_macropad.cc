#include "adafruit_macropad/adafruit_macropad.h"

#include <memory>

#include "font/dotmatrix_font.h"
#include "hardware/gpio.h"
#include "sh1106_spi/sh1106_spi.h"

namespace crynsnd {

using ::crynsnd::fonts::DotMatrix;

AdafruitMacropad::AdafruitMacropad() : initialized_(false), display_(nullptr) {}

AdafruitMacropad::~AdafruitMacropad() {}

void AdafruitMacropad::init() {
  initKeys();
  initDisplay();
  initialized_ = true;
}

void AdafruitMacropad::initKeys() {
  gpio_init_mask(MACROPAD_ALL_BUTTONS_GPIO_MASK);
  gpio_set_dir_masked(MACROPAD_ALL_BUTTONS_GPIO_MASK, GPIO_IN);
  // GPIO 0 to 12 are pull-up.
  for (int i = 0; i <= 12; i += 1) {
    gpio_pull_up(i);
  }
}

void AdafruitMacropad::initDisplay() {
  std::unique_ptr<SH1106> sh1106 = std::make_unique<SH1106>(
      spi1, MACROPAD_DISPLAY_MISO, MACROPAD_DISPLAY_MOSI, MACROPAD_DISPLAY_SCK,
      MACROPAD_DISPLAY_CS, MACROPAD_DISPLAY_RESET, MACROPAD_DISPLAY_DC);
  sh1106->init();

  std::unique_ptr<DotMatrix> font = std::make_unique<DotMatrix>();

  display_ =
      std::make_unique<DisplayManager>(std::move(sh1106), std::move(font));
}

bool AdafruitMacropad::isKeyPressed(uint key) { return !gpio_get(key); }

DisplayManager& AdafruitMacropad::getDisplay() { return *display_; }

}  // namespace crynsnd
