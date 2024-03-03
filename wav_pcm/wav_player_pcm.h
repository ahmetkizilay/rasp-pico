#ifndef __crynsnd_wav_pcm_wav_player_pcm_h
#define __crynsnd_wav_pcm_wav_player_pcm_h

#include <cstdint>

#include "wav_pcm/wav_parser.h"

namespace crynsnd {

class WavPlayerPcm {
 public:
  explicit WavPlayerPcm(int pwm_gpio);
  ~WavPlayerPcm();

  void play(const crynsnd::WaveFile* file, bool loop = false);
  void stop();

  bool isPlaying();

 private:
  const int pwm_gpio_;
  int slice_num_;
};
}  // namespace crynsnd
#endif  // __crynsnd_wav_pcm_wav_player_pcm_h