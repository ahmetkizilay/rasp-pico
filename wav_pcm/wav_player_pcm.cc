#include "wav_pcm/wav_player_pcm.h"

#include "hardware/gpio.h"
#include "hardware/pwm.h"

namespace crynsnd {
namespace {

// declaring these as global variables so that they can be accessed in the
// interrupt handler.
// How many times we want to play each sample.
uint8_t reps = 4;
// current repetition count
uint8_t rep_count = 0;
// how much to increment the position by.
// For 8-bit samples, this is 1, and for 16-bit samples, this is 2.
uint8_t pos_increment = 0;

uint32_t wav_position = 0;
uint32_t wav_length = 0;
const uint8_t* wav_data = nullptr;
uint16_t wav_bits_per_sample = 0;

bool done_playing = true;
bool is_looping = false;
int pwm_pin = 20;

// This function is called when the PWM peripheral wraps around.
// We use this function to send the next sample to the PWM peripheral.
void on_pwm_wrap0() {
  
  pwm_clear_irq(pwm_gpio_to_slice_num(pwm_pin));
  if (done_playing) {
    pwm_set_enabled(pwm_gpio_to_slice_num(pwm_pin), false);
    return;
  }

  if (wav_position < wav_length) {
    if (wav_bits_per_sample == 8) {
      pwm_set_gpio_level(pwm_pin, wav_data[wav_position]);

    } else if (wav_bits_per_sample == 16) {
      int16_t sample = wav_data[wav_position] | (wav_data[wav_position + 1] << 8);
      sample = sample / 256;
      pwm_set_gpio_level(pwm_pin, static_cast<uint8_t>(sample + 0x80));
    }

    rep_count += 1;
    if (rep_count == reps) {
      wav_position += pos_increment;
      rep_count = 0;
    }
  } else {
    if (is_looping) {
      wav_position = 0;
    } else {
      done_playing = true;
    }
  }
}
}  // namespace

WavPlayerPcm::WavPlayerPcm(int pwm_gpio) : pwm_gpio_(pwm_gpio) {
  pwm_pin = pwm_gpio;
  reps = 4;
  rep_count = 0;
  pos_increment = 0;
  wav_position = 0;
  done_playing = true;

  wav_length = 0;
  wav_data = nullptr;
  wav_bits_per_sample = 0;

  // initialize the PWM pin
  gpio_set_function(pwm_gpio_, GPIO_FUNC_PWM);
  slice_num_ = pwm_gpio_to_slice_num(pwm_gpio_);

  // set pwm interrupt
  pwm_clear_irq(slice_num_);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap0);
  pwm_set_irq_enabled(slice_num_, true);
  irq_set_enabled(PWM_IRQ_WRAP, true);
}

WavPlayerPcm::~WavPlayerPcm() {
  // disable the PWM interrupt
  pwm_set_irq_enabled(slice_num_, false);
  irq_set_enabled(PWM_IRQ_WRAP, false);
}

void WavPlayerPcm::play(const WaveFile* file, bool loop) {
  wav_length = file->GetDataSize();
  wav_data = file->GetData();
  wav_bits_per_sample = file->GetWaveFormat().bits_per_sample;

  if (file->GetWaveFormat().bits_per_sample == 8) {
    pos_increment = 1;
  } else if (file->GetWaveFormat().bits_per_sample == 16) {
    pos_increment = 2;
  }

  int sample_rate_multiplier = 1;
  switch (file->GetWaveFormat().sample_rate) {
    case 44100:
      sample_rate_multiplier = 1;
      break;
    case 22050:
      sample_rate_multiplier = 2;
      break;
    case 11025:
      sample_rate_multiplier = 4;
      break;
    default:
      break;
  }
  wav_position = 0;
  done_playing = false;
  is_looping = loop;

  pwm_config config = pwm_get_default_config();
  // The clock speed is 176000 kHz, and the wrap value is 250.
  // The frequency of the PWM signal is 176000 / 250 = 704 kHz.
  // With clock divider of 8, the frequency of the PWM signal is 704 / 8 = 88
  // kHz. We play each sample 4 times, so the frequency of the audio signal is
  // 88 / 4 = 22 kHz.
  pwm_config_set_clkdiv(&config, 4.0f * sample_rate_multiplier);
  pwm_config_set_wrap(&config, 250);
  pwm_init(slice_num_, &config, true);
}

void WavPlayerPcm::stop() {
  pwm_set_enabled(slice_num_, false);
  wav_position = 0;
}

bool WavPlayerPcm::isPlaying() {
  return !done_playing;
}
}  // namespace crynsnd
