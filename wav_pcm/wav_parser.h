#ifndef __crynsnd_wav_pcm_wav_parser_h
#define __crynsnd_wav_pcm_wav_parser_h

#include <cstdint>
#include <memory>

namespace crynsnd {

struct WaveFormat {
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;
  uint16_t min_sample;
  uint16_t max_sample;
};

class WaveFile {
 public:
  static std::unique_ptr<WaveFile> Create(const uintptr_t wav);

  WaveFile(WaveFormat wave_format, uint32_t data_size, const uintptr_t address)
      : wave_format_(wave_format), data_size_(data_size), address_(address) {}

  const WaveFormat &GetWaveFormat() const { return wave_format_; }
  const uint32_t GetDataSize() const { return data_size_; }
  const uint8_t *GetData() const { return (const uint8_t*) address_; }

 private:
  const WaveFormat wave_format_;
  const uint32_t data_size_;
  const uintptr_t address_;
};

}  // namespace crynsnd

#endif  // __crynsnd_wav_pcm_wav_parser_h
