#ifndef __crynsnd_ws2812_pio_ws2812_pio_h
#define __crynsnd_ws2812_pio_ws2812_pio_h

#include "hardware/pio.h"

namespace crynsnd {
class WS2812 {
 public:
  static WS2812 Init(PIO& pio, uint sm, uint pin);

  void put_blocking(uint32_t data);

 private:
  explicit WS2812(PIO& pio, uint sm) : pio_(pio), sm_(sm) {}

  PIO& pio_;
  uint sm_;
};

}  // namespace crynsnd
#endif  // __crynsnd_ws2812_pio_ws2812_pio_h