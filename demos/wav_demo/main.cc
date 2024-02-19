#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "wav_pcm/wav_parser.h"

extern char __flash_binary_end;

int main() {
  stdio_init_all();

  // wait for enter char to start
  getchar();

  // __flash_binary_end__ gives us the binary size. We want to align the data to
  // th next next 4k boundary after the end of the binary. This is because the
  // flash is organized in 4k sectors.
  uintptr_t binary_end = (uintptr_t)&__flash_binary_end;
  const uintptr_t flash_offset = ((binary_end >> 12) + 1) << 12;

  uint8_t *flash_target_contents = (uint8_t *)(flash_offset);

  printf("This is where binary ends: 0x%X\n", binary_end);
  printf("This is the flash offset: 0x%X\n", flash_offset);

  printf("Press enter to continue...\n");
  getchar();

  std::unique_ptr<crynsnd::WaveFile> wave_file =
      crynsnd::WaveFile::Create(flash_target_contents);

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
  printf("data_size: %d\n", wave_file->GetDataSize());

  while (1) {
    sleep_ms(5000);
  }
}
