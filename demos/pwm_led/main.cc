#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"

void on_pwm_wrap() {
  static int fade = 0;
  static bool going_up = true;

  // clear the interrupt
  pwm_clear_irq(pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN));

  if (going_up) {
    ++fade;
    if (fade > 255) {
      fade = 255;
      going_up = false;
    }
  } else {
    --fade;
    if (fade < 0) {
      fade = 0;
      going_up = true;
    }
  }

  // fade value is squared for a more visible effect. 
  // The function accepts u16 bit value.
  pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, fade * fade);
}

int main() {
  uint pin = PICO_DEFAULT_LED_PIN;

  // set the PWM pin
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);

  // Set the PWM interrup
  pwm_clear_irq(slice_num);
  pwm_set_irq_enabled(slice_num, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  // Set pwm config and enable it
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 4.f);
  pwm_init(slice_num, &config, true);

  // The action happens in PWM interrupt handler.
  // on_pwm_wrap function.
  while (1) {
    tight_loop_contents();
  }
}
