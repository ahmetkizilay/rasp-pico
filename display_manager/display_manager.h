#ifndef __crynsnd_display_manager_display_manager_h
#define __crynsnd_display_manager_display_manager_h

#include <string>

#include "font/font.h"
#include "sh1106_spi/sh1106_spi.h"

#define DISPLAY_LETTER_SPACING 1
namespace crynsnd {

class DisplayManager {
 public:
  explicit DisplayManager(SH1106* sh1106, fonts::Font* font);

  void write(const std::string& text);
  void clear();
 private:
  SH1106* sh1106_;
  fonts::Font* font_;
};

}  // namespace crynsnd
#endif  // __crynsnd_display_manager_display_manager_h