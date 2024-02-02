#include "sh1106_spi/sh1106_spi.h"

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/time.h"

namespace crynsnd {

SH1106::~SH1106() {
  if (buffer_ != nullptr) {
    free(buffer_);
    buffer_ = nullptr;
  }
  if (write_pad_ != nullptr) {
    free(write_pad_);
    write_pad_ = nullptr;
  }
}

void SH1106::writePad() { spi_write_blocking(spi_, write_pad_, 2); }

void SH1106::writeCommand(uint8_t* data, size_t len) {
  asm volatile("nop \n nop \n nop");
  gpio_put(dc_pin_, 0);
  asm volatile("nop \n nop \n nop");
  spi_write_blocking(spi_, data, len);
  asm volatile("nop \n nop \n nop");
}

void SH1106::writeCommand(uint8_t command) {
  asm volatile("nop \n nop \n nop");
  gpio_put(dc_pin_, 0);
  asm volatile("nop \n nop \n nop");
  spi_write_blocking(spi_, &command, 1);
  asm volatile("nop \n nop \n nop");
  gpio_put(dc_pin_, 1);
  asm volatile("nop \n nop \n nop");
}

void SH1106::init() {
  spi_init(spi_, 1000 * 1000);

  gpio_set_function(rx_pin_, GPIO_FUNC_SPI);
  gpio_set_function(sck_pin_, GPIO_FUNC_SPI);
  gpio_set_function(tx_pin_, GPIO_FUNC_SPI);

  // active low
  gpio_init(cs_pin_);
  gpio_set_dir(cs_pin_, GPIO_OUT);
  gpio_put(cs_pin_, 0);

  // active low
  gpio_init(reset_pin_);
  gpio_set_dir(reset_pin_, GPIO_OUT);
  gpio_put(reset_pin_, 1);

  gpio_init(dc_pin_);
  gpio_set_dir(dc_pin_, GPIO_OUT);
  gpio_put(dc_pin_, 0);

  // boot sequence
  // Hold ^reset for 100us
  gpio_put(reset_pin_, 1);
  sleep_us(100);
  gpio_put(reset_pin_, 0);
  sleep_us(100);
  gpio_put(reset_pin_, 1);

  // References:
  // https://github.com/dlkj/adafruit-macropad-rp2040-getting-started/blob/main/display_test/display_test.c
  // https://github.com/adafruit/Adafruit_SH110x/blob/master/Adafruit_SH1106G.cpp
  uint8_t buf[] = {
      0xAE,        // display off
      0xD5, 0x80,  // clock div
      0xA8, 0x3F,  // multiplex
      0xD3, 0x00,  // display offset
      0xAD, 0x8B,  // charge pump
      0xA1,        // segremap
      0xC8,        // com scan
      0xDA, 0x12,  // com pins
      0x81, 0xFF,  // contrast
      0xD9, 0x1F,  // precharge
      0xDB, 0x40,  // vcom detect
      0x33,        // vpp to 9v
      0xA6,        // normal display
      0x20, 0x10,  // memory mode
      0xA4         // displayall on resume
  };
  writeCommand(buf, sizeof(buf));

  sleep_ms(100);
  writeCommand(0xAF);  // display on
}

void SH1106::clearDisplay() {
  memset(buffer_, 0, buffer_size_);
  dirty_ = 255;
}

void SH1106::setPixel(uint8_t x, uint8_t y) {
  uint8_t page = (y / 8);
  uint8_t offset = (y % 8);
  buffer_[(x + (SH1106_PADDING / 2)) +
          (page * (SH1106_WIDTH + SH1106_PADDING))] |= (1 << offset);
  dirty_ |= (1 << page);
}

void SH1106::setData(uint8_t x, uint8_t y, const uint8_t* val, size_t len) {
  uint16_t offset =
      x + (SH1106_PADDING / 2) + (y * (SH1106_WIDTH + SH1106_PADDING));
  memcpy(&buffer_[offset], val, len);
  dirty_ |= (1 << y);
}

void SH1106::flush() {
  for (int j = 0; j < SH1106_PAGE_COUNT; j += 1) {
    if ((dirty_ & (1 << j)) == 0) {
      continue;
    }
    writeCommand(0xB0 | j);

    spi_write_blocking(spi_, &buffer_[j * (SH1106_WIDTH + SH1106_PADDING)],
                       SH1106_WIDTH + SH1106_PADDING);
  }
  dirty_ = 0;
}

}  // namespace crynsnd