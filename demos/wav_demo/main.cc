#include <stdio.h>
#include <string.h>

#include "extra_data.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "wav_pcm/wav_parser.h"

#define TODBOT_OUT 20

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
const uint8_t *wav_data = nullptr;
uint16_t wav_bits_per_sample = 0;


void on_pwm_wrap() {
  // clear the interrupt
  pwm_clear_irq(pwm_gpio_to_slice_num(TODBOT_OUT));

  if (wav_position < wav_length) {
    if (wav_bits_per_sample == 8) {
      pwm_set_gpio_level(TODBOT_OUT, wav_data[wav_position]);

    } else if (wav_bits_per_sample == 16) {
      int16_t sample =
          wav_data[wav_position] | (wav_data[wav_position + 1] << 8);
      sample = sample / 256;
      pwm_set_gpio_level(TODBOT_OUT, static_cast<uint8_t>(sample + 0x80));
    }

    rep_count += 1;
    if (rep_count == reps) {
      wav_position += pos_increment;
      rep_count = 0;
    }
  } else {
    wav_position = 0;
  }
}

int main() {
  // This clock speed makes it easier to set the correct frequency for the PWM
  set_sys_clock_khz(176000, true);

  stdio_init_all();

  const crynsnd::DataDescriptor wav_files[] = {
      crynsnd::GetDataDescriptor("file1.wav"),
      crynsnd::GetDataDescriptor("file2.wav"),
      crynsnd::GetDataDescriptor("file3.wav"),
  };

  printf("Press enter to continue...\n");
  getchar();

  for (const crynsnd::DataDescriptor &wav_file : wav_files) {
    std::unique_ptr<crynsnd::WaveFile> wave_file =
        crynsnd::WaveFile::Create(wav_file.address);

    if (wave_file == nullptr) {
      printf("error reading wave format");
    }

    const crynsnd::WaveFormat &wave_format = wave_file->GetWaveFormat();

    printf("audio_format: %d\n", wave_format.audio_format);
    printf("num_channels: %d\n", wave_format.num_channels);
    printf("sample_rate: %d\n", wave_format.sample_rate);
    printf("byte_rate: %d\n", wave_format.byte_rate);
    printf("block_align: %d\n", wave_format.block_align);
    printf("bits_per_sample: %d\n", wave_format.bits_per_sample);
    printf("min_sample: %x\n", wave_format.min_sample);
    printf("max_sample: %x\n", wave_format.max_sample);
    printf("data_size: %d\n", wave_file->GetDataSize());
    printf("\n");
  }

  // Setting up the PWM for file3.wav
  // read a character and store it in a variable called file_index
  int file_index = -1;
  while (file_index == -1) {
    printf("Enter the file index (0, 1, 2): ");
    int received = getchar() - '0';
    if (received >= 0 && received <= 2) {
      file_index = received;
      break;
    } else {
      printf("Invalid file index\n");
    }
  }
  printf("Playing file: %d\n", file_index);

  std::unique_ptr<crynsnd::WaveFile> wav_file =
      crynsnd::WaveFile::Create(wav_files[file_index].address);

  wav_position = 0;
  wav_length = wav_file->GetDataSize();
  wav_data = wav_file->GetData();
  wav_bits_per_sample = wav_file->GetWaveFormat().bits_per_sample;
  // set pos_increment based on the bits per sample
  pos_increment = wav_bits_per_sample / 8;
  
  int sample_rate_multiplier = 1;
  switch (wav_file->GetWaveFormat().sample_rate) {
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


  // Initialize the PWM Pin
  gpio_set_function(TODBOT_OUT, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(TODBOT_OUT);

  // Set the PWM interrupt
  pwm_clear_irq(slice_num);
  pwm_set_irq_enabled(slice_num, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  // Set pwm config and enable it
  pwm_config config = pwm_get_default_config();
  // The clock speed is 176000 kHz, and the wrap value is 250.
  // The frequency of the PWM signal is 176000 / 250 = 704 kHz.
  // With clock divider of 8, the frequency of the PWM signal is 704 / 8 = 88
  // kHz. We play each sample 4 times, so the frequency of the audio signal is
  // 88 / 4 = 22 kHz.
  pwm_config_set_clkdiv(&config, 4.0f * sample_rate_multiplier);
  pwm_config_set_wrap(&config, 250);
  pwm_init(slice_num, &config, true);

  pwm_set_gpio_level(TODBOT_OUT, 0);

  // Setting up the PWM for file3.wav
  
  std::unique_ptr<crynsnd::WaveFile> wav_file =
      crynsnd::WaveFile::Create(wav_files[2].address);

  wav_position = 0;
  wav_length = wav_file->GetDataSize();
  wav_data = wav_file->GetData();

  // Initialize the PWM Pin
  gpio_set_function(TODBOT_OUT, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(TODBOT_OUT);

  // Set the PWM interrupt
  pwm_clear_irq(slice_num);
  pwm_set_irq_enabled(slice_num, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  // Set pwm config and enable it
  pwm_config config = pwm_get_default_config();
  // The clock speed is 176000 kHz, and the wrap value is 250.
  // The frequency of the PWM signal is 176000 / 250 = 704 kHz.
  // With clock divider of 8, the frequency of the PWM signal is 704 / 8 = 88 kHz.
  // We play each sample 4 times, so the frequency of the audio signal is 88 / 4 = 22 kHz.
  pwm_config_set_clkdiv(&config, 8.0f);
  pwm_config_set_wrap(&config, 250);
  pwm_init(slice_num, &config, true);

  pwm_set_gpio_level(TODBOT_OUT, 0);

  while (1) {
    tight_loop_contents();
  }
}
