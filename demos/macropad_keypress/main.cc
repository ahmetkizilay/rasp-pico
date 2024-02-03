#include "adafruit_macropad/adafruit_macropad.h"
#include "display_manager/display_manager.h"
#include "pico/time.h"

using ::crynsnd::AdafruitMacropad;
using ::crynsnd::DisplayManager;

int main() {
  AdafruitMacropad macropad;
  macropad.init();

  DisplayManager& display = macropad.getDisplay();
  while (true) {
    display.bufferClear();
    if (macropad.isKeyPressed(MACROPAD_KEY_1)) {
      display.bufferWrite(0, 3, "KEY 1");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_2)) {
      display.bufferWrite(45, 3, "KEY 2");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_3)) {
      display.bufferWrite(90, 3, "KEY 3");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_4)) {
      display.bufferWrite(0, 4, "KEY 4");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_5)) {
      display.bufferWrite(45, 4, "KEY 5");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_6)) {
      display.bufferWrite(90, 4, "KEY 6");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_7)) {
      display.bufferWrite(0, 5, "KEY 7");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_8)) {
      display.bufferWrite(45, 5, "KEY 8");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_9)) {
      display.bufferWrite(90, 5, "KEY 9");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_10)) {
      display.bufferWrite(0, 6, "KEY 10");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_11)) {
      display.bufferWrite(45, 6, "KEY 11");
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_12)) {
      display.bufferWrite(90, 6, "KEY 12");
    }
    display.bufferFlush();
    sleep_ms(33);
  }
}