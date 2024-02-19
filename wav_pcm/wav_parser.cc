#include "wav_pcm/wav_parser.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>

namespace crynsnd {

// static
std::unique_ptr<WaveFile> WaveFile::Create(const uint8_t *cursor) {
  // print the next four chars from cursor.
  // the output should look like "RIFF"
  char tag[5] = {0};
  for (int i = 0; i < 4; i += 1) {
    tag[i] = *(cursor + i);
  }
  if (strcmp(tag, "RIFF") != 0) {
    printf("Expected RIFF, but found: %s\n", tag);
    return nullptr;
  }
  cursor += 4;

  // next four bytes from cursor should be the size of the file.
  // we don't need to do anything with this value for now.
  cursor += 4;

  // next four chars from cursor should look like "WAVE"
  for (int i = 0; i < 4; i += 1) {
    tag[i] = *(cursor + i);
  }
  if (strcmp(tag, "WAVE") != 0) {
    printf("Expected WAV, but found: %s\n", tag);
    return nullptr;
  }
  cursor += 4;

  // next four chars from cursor should look like "fmt "
  for (int i = 0; i < 4; i += 1) {
    tag[i] = *(cursor + i);
  }
  cursor += 4;

  if (strcmp(tag, "fmt ") != 0) {
    printf("Expected fmt, but found: %s\n", tag);
    return nullptr;
  }

  // next four bytes is the size of the fmt chunk.
  // the output is a 32-bit integer in little-endian format.
  // fmt-chunk size is 16 for PCM.
  uint32_t fmt_size = 0;
  for (int i = 0; i < 4; i += 1) {
    fmt_size |= (*(cursor + i) << (i * 8));
  }
  cursor += 4;
  if (fmt_size != 16) {
    printf("unexpected fmt_size: %d. Expected 16.\n", fmt_size);
    return nullptr;
  }

  // next two bytes is the audio format.
  // the output is a 16-bit integer in little-endian format.
  // the output should be 1 for PCM.
  uint16_t audio_format = 0;
  for (int i = 0; i < 2; i += 1) {
    audio_format |= (*(cursor + i) << (i * 8));
  }
  cursor += 2;

  if (audio_format != 1) {
    printf("Sorry, currently only support PCM. Received: %d.\n", audio_format);
    return nullptr;
  }
  if (audio_format == 1 && fmt_size != 16) {
    printf("For PCM expected size of 16, but found %d.\n", fmt_size);
    return nullptr;
  }

  // next two bytes is the number of channels.
  // the output is a 16-bit integer in little-endian format.
  // the output is the number of channels.
  uint16_t num_channels = 0;
  for (int i = 0; i < 2; i += 1) {
    num_channels |= (*(cursor + i) << (i * 8));
  }
  cursor += 2;

  // next four bytes is the sample rate.
  // the output is a 32-bit integer in little-endian format.
  // the output is the sample rate.
  uint32_t sample_rate = 0;
  for (int i = 0; i < 4; i += 1) {
    sample_rate |= (*(cursor + i) << (i * 8));
  }
  cursor += 4;

  // next four bytes is the byte rate.
  // the output is a 32-bit integer in little-endian format.
  // the output is the byte rate.
  uint32_t byte_rate = 0;
  for (int i = 0; i < 4; i += 1) {
    byte_rate |= (*(cursor + i) << (i * 8));
  }
  cursor += 4;

  // next two bytes is the block align.
  // the output is a 16-bit integer in little-endian format.
  // the output is the block align.
  uint16_t block_align = 0;
  for (int i = 0; i < 2; i += 1) {
    block_align |= (*(cursor + i) << (i * 8));
  }
  cursor += 2;

  // next two bytes is the bits per sample.
  // the output is a 16-bit integer in little-endian format.
  // the output is the bits per sample.
  uint16_t bits_per_sample = 0;
  for (int i = 0; i < 2; i += 1) {
    bits_per_sample |= (*(cursor + i) << (i * 8));
  }
  cursor += 2;

  // read the next tag to see if it's "data"
  for (int i = 0; i < 4; i += 1) {
    tag[i] = *(cursor + i);
  }
  cursor += 4;
  if (strcmp(tag, "data") != 0) {
    printf("Expected data, but found: %s\n", tag);
    return nullptr;
  }

  // next four bytes is the size of the data chunk.
  // the output is a 32-bit integer in little-endian format.
  // the output is the size of the data chunk.
  uint32_t data_size = 0;
  for (int i = 0; i < 4; i += 1) {
    data_size |= (*(cursor + i) << (i * 8));
  }
  cursor += 4;

  const WaveFormat wave_format{
      .audio_format = audio_format,
      .num_channels = num_channels,
      .sample_rate = sample_rate,
      .byte_rate = byte_rate,
      .block_align = block_align,
      .bits_per_sample = bits_per_sample,
  };

  return std::make_unique<WaveFile>(wave_format, data_size, cursor);
}

}  // namespace crynsnd
