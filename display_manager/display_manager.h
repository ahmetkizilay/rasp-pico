#ifndef __crynsnd_display_manager_display_manager_h
#define __crynsnd_display_manager_display_manager_h

#include <memory>
#include <string>

#include "font/font.h"
#include "sh1106_spi/sh1106_spi.h"

#define DISPLAY_LETTER_SPACING 1
namespace crynsnd {

class DisplayManager {
 public:
  explicit DisplayManager(std::unique_ptr<SH1106> sh1106,
                          std::unique_ptr<fonts::Font> font);

  void write(const std::string& text);
  void write(uint x, uint y, const std::string& text, bool commit = true);
  void clear(bool commit = true);
  inline void flush() { sh1106_->flush(); }

 private:
  std::unique_ptr<SH1106> sh1106_;
  std::unique_ptr<fonts::Font> font_;
};

}  // namespace crynsnd
#endif  // __crynsnd_display_manager_display_manager_h