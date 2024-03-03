#ifndef __crynsnd_adafruit_macropad_adafruit_macropad_h
#define __crynsnd_adafruit_macropad_adafruit_macropad_h
#include <memory>

#include "display_manager/display_manager.h"

#define MACROPAD_LED 13
#define MACROPAD_NEOPIXEL 19

#define MACROPAD_SDA 20
#define MACROPAD_SCL 21

#define MACROPAD_DISPLAY_CS 22
#define MACROPAD_DISPLAY_RESET 23
#define MACROPAD_DISPLAY_DC 24
#define MACROPAD_DISPLAY_SCK 26
#define MACROPAD_DISPLAY_MOSI 27
#define MACROPAD_DISPLAY_MISO 28

#define MACROPAD_KEY_1 1
#define MACROPAD_KEY_2 2
#define MACROPAD_KEY_3 3
#define MACROPAD_KEY_4 4
#define MACROPAD_KEY_5 5
#define MACROPAD_KEY_6 6
#define MACROPAD_KEY_7 7
#define MACROPAD_KEY_8 8
#define MACROPAD_KEY_9 9
#define MACROPAD_KEY_10 10
#define MACROPAD_KEY_11 11
#define MACROPAD_KEY_12 12
#define MACROPAD_ALL_BUTTONS_GPIO_MASK 0b1111111111111

namespace crynsnd {

class AdafruitMacropad {
 public:
  explicit AdafruitMacropad();
  ~AdafruitMacropad();

  void init();
  bool isKeyPressed(uint key);

  DisplayManager& getDisplay();

 private:
  bool initialized_;
  std::unique_ptr<DisplayManager> display_;

  void initKeys();
  void initDisplay();
};
}  // namespace crynsnd

#endif  // __crynsnd_adafruit_macropad_adafruit_macropad_h