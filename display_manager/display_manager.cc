#include "display_manager/display_manager.h"

#include <memory>
#include <string>

#include "font/font.h"
#include "sh1106_spi/sh1106_spi.h"

namespace crynsnd {
using ::crynsnd::fonts::Font;

DisplayManager::DisplayManager(std::unique_ptr<SH1106> sh1106,
                               std::unique_ptr<Font> font)
    : sh1106_(std::move(sh1106)), font_(std::move(font)) {}

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

void DisplayManager::write(uint x, uint y, const std::string& text,
                           bool commit) {
  for (int i = 0; i < text.length(); i++) {
    const int index = font_->GetLetterIndex(text[i]);
    const int width = font_->GetLetterWidth(text[i]);
    // Prevent overflow
    // TODO: get the available width from the object.
    if (x + width >= 128) {
      break;
    }
    sh1106_->setData(x, y, &font_->font[index], width);
    x += (width + DISPLAY_LETTER_SPACING);
  }
  if (commit) {
    flush();
  }
}

void DisplayManager::clear(bool commit) {
  sh1106_->clearDisplay();
  if (commit) {
    flush();
  }
}

}  // namespace crynsnd
