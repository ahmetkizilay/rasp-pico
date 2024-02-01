#ifndef __crynsnd_sh1106_spi_sh1106_spi_h
#define __crynsnd_sh1106_spi_sh1106_spi_h

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>

#include "hardware/spi.h"

#define SH1106_PAGE_COUNT 8
#define SH1106_WIDTH 128
#define SH1106_PADDING 4

namespace crynsnd {

// SH1106 OLED display driver.
class SH1106 {
 public:
  explicit SH1106(spi_inst_t* spi, uint8_t rx_pin, uint8_t tx_pin,
                  uint8_t sck_pin, uint8_t cs_pin, uint8_t reset_pin,
                  uint8_t dc_pin)
      : spi_(spi),
        rx_pin_(rx_pin),
        tx_pin_(tx_pin),
        sck_pin_(sck_pin),
        cs_pin_(cs_pin),
        reset_pin_(reset_pin),
        dc_pin_(dc_pin),
        // +2 for padding in both sides
        buffer_size_((SH1106_WIDTH + SH1106_PADDING) * SH1106_PAGE_COUNT),
        dirty_(0) {
    buffer_ = (uint8_t*)malloc(buffer_size_);
  }

  ~SH1106();

  void init();
  void clearDisplay();
  void setPixel(uint8_t x, uint8_t y);

  void flush();

 private:
  spi_inst_t* spi_;
  uint8_t rx_pin_;
  uint8_t tx_pin_;
  uint8_t sck_pin_;
  uint8_t cs_pin_;
  uint8_t reset_pin_;
  uint8_t dc_pin_;

  uint32_t buffer_size_;
  uint8_t dirty_;
  uint8_t* buffer_;
  uint8_t* write_pad_;

  void writeCommand(uint8_t command);
  void writeCommand(uint8_t* data, size_t len);
  void writePad();
};

}  // namespace crynsnd
#endif  // __crynsnd_sh1106_spi_sh1106_spi_h