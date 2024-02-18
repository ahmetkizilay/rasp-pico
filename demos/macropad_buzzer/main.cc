#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "sample.h"


#define MACROPAD_BUZZER 16
#define MACROPAD_BUZZER_SHUTDOWN 14

int wav_position = 0;

void on_pwm_wrap() {
  // clear the interrupt
  pwm_clear_irq(pwm_gpio_to_slice_num(MACROPAD_BUZZER));

  if (wav_position < (WAV_DATA_LENGTH<<1) - 1) {
    pwm_set_gpio_level(MACROPAD_BUZZER, WAV_DATA[wav_position>>1]);
    wav_position++;
  } else {
    wav_position = 0;
  }
}

int main() {

  set_sys_clock_khz(176000, true); 

  // Enable the speaker
  gpio_init(MACROPAD_BUZZER_SHUTDOWN);
  gpio_set_dir(MACROPAD_BUZZER_SHUTDOWN, GPIO_OUT);
  gpio_put(MACROPAD_BUZZER_SHUTDOWN, 1);

  // set the PWM pin
  gpio_set_function(MACROPAD_BUZZER, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(MACROPAD_BUZZER);

  // Set the PWM interrup
  pwm_clear_irq(slice_num);
  pwm_set_irq_enabled(slice_num, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  // Set pwm config and enable it
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 16.0f);
  pwm_config_set_wrap(&config, 250);
  pwm_init(slice_num, &config, true);

  pwm_set_gpio_level(MACROPAD_BUZZER, 0);

  // The action happens in PWM interrupt handler.
  // on_pwm_wrap function.
  while (1) {
    tight_loop_contents();
  }
}


