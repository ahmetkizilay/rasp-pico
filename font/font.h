#ifndef __crynsnd_font_h__
#define __crynsnd_font_h__

#include <cstdio>

namespace crynsnd {
namespace fonts {

class Font {
 public:
  explicit Font(const uint8_t* font_data) : font(font_data) {}

  virtual const uint8_t GetLetterWidth(char val) const = 0;
  virtual const uint16_t GetLetterIndex(char val) const = 0;
  const uint8_t* font;
};

}  // namespace fonts
}  // namespace crynsnd

#endif  // __crynsnd_font_h__
