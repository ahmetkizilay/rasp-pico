#include "adafruit_macropad/adafruit_macropad.h"
#include "display_manager/display_manager.h"
#include "pico/time.h"

using ::crynsnd::AdafruitMacropad;
using ::crynsnd::DisplayBuffer;
using ::crynsnd::DisplayManager;

int main() {
  AdafruitMacropad macropad;
  macropad.init();

  DisplayManager& display = macropad.getDisplay();
  while (true) {
    display.clear(/*commit=*/false);
    if (macropad.isKeyPressed(MACROPAD_KEY_1)) {
      display.write(0, 3, "KEY 1", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_2)) {
      display.write(45, 3, "KEY 2", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_3)) {
      display.write(90, 3, "KEY 3", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_4)) {
      display.write(0, 4, "KEY 4", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_5)) {
      display.write(45, 4, "KEY 5", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_6)) {
      display.write(90, 4, "KEY 6", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_7)) {
      display.write(0, 5, "KEY 7", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_8)) {
      display.write(45, 5, "KEY 8", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_9)) {
      display.write(90, 5, "KEY 9", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_10)) {
      display.write(0, 6, "KEY 10", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_11)) {
      display.write(45, 6, "KEY 11", /*commit=*/false);
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_12)) {
      display.write(90, 6, "KEY 12", /*commit=*/false);
    }
    display.flush();
    sleep_ms(33);
  }
}