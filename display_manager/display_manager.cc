#include "display_manager/display_manager.h"

#include <string>

#include "font/font.h"
#include "sh1106_spi/sh1106_spi.h"

namespace crynsnd {
using ::crynsnd::fonts::Font;

DisplayManager::DisplayManager(SH1106* sh1106, Font* font)
    : sh1106_(sh1106), font_(font) {}

void DisplayManager::write(const std::string& text) {
  int x = 0;
  for (int i = 0; i < text.length(); i++) {
    const int index = font_->GetLetterIndex(text[i]);
    const int width = font_->GetLetterWidth(text[i]);
    // Prevent overflow
    // TODO: get the available width from the object.
    if (x + width >= 128) {
      break;
    } 
    sh1106_->setData(x, 0, &font_->font[index], width);
    x += (width + DISPLAY_LETTER_SPACING);
  }
  sh1106_->flush();
}

void DisplayManager::clear() {
  sh1106_->clearDisplay();
  sh1106_->flush();
}

}  // namespace crynsnd
