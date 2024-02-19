#include <stdio.h>
#include <string.h>

#include "extra_data.h"
#include "pico/stdlib.h"
#include "wav_pcm/wav_parser.h"

extern char __flash_binary_end;

int main() {
  stdio_init_all();

  // initialize DataDescriptor array from file1.wav and file2.wav
  const crynsnd::DataDescriptor wav_files[] = {
      crynsnd::GetDataDescriptor("file1.wav"),
      crynsnd::GetDataDescriptor("file2.wav"),
  };

  printf("Press enter to continue...\n");
  getchar();

  for (const crynsnd::DataDescriptor &wav_file : wav_files) {
    std::unique_ptr<crynsnd::WaveFile> wave_file =
        crynsnd::WaveFile::Create((const uint8_t *)wav_file.address);

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
    printf("\n");
  }


  while (1) {
    sleep_ms(5000);
  }
}
