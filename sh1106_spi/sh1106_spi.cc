#include "sh1106_spi/sh1106_spi.h"

#include <stddef.h>
#include <stdint.h>

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/time.h"

namespace crynsnd {

void SH1106::writeData(uint8_t val) { spi_write_blocking(spi_, &val, 1); }

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
  for (int j = 0; j < SH1106_PAGE_COUNT; j += 1) {
    writeCommand(0xB0 | j);

    writeData(0);
    writeData(0);

    for (int i = 0; i < SH1106_WIDTH; i += 1) {
      writeData(0);
    }
    writeData(0);
    writeData(0);
  }
}

void SH1106::write() {
  writeCommand(0xB5);

  writeData(0);
  writeData(0);

  for (int i = 0; i < SH1106_WIDTH; i += 1) {
    writeData(0b11111111);
  }

  writeData(0);
  writeData(0);
}

}  // namespace crynsnd