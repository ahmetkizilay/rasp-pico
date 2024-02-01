#include "font/dotmatrix_font.h"

namespace crynsnd {
namespace fonts {

DotMatrix::DotMatrix() : Font{FONT} {};

const uint16_t DotMatrix::GetLetterIndex(char val) const {
  uint16_t i = 0;
  for (; i < ALPHABET_SIZE; i += 1) {
    if (ALL_GLYPHS[i] == val) {
      return i * LETTER_WIDTH;
    }
  }
  return i * LETTER_WIDTH;
}

}  // namespace fonts
}  // namespace crynsnd