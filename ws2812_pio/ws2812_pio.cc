#include "ws2812_pio/ws2812_pio.h"

#include "hardware/pio.h"
#include "ws2812.pio.h"

namespace crynsnd {

// static
WS2812 WS2812::Init(PIO& pio, uint sm, uint pin) {
  // TODO(): Complete
  uint offset = pio_add_program(pio, &ws2812_program);

  ws2812_program_init(pio, sm, offset, pin, 800000, false);

  return WS2812(pio, sm);
}

void WS2812::put_blocking(uint32_t data) {
  pio_sm_put_blocking(pio_, sm_, data);
}

}  // namespace crynsnd