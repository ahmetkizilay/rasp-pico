#ifndef __crynsnd_wav_player_wav_player_pwm_h
#define __crynsnd_wav_player_wav_player_pwm_h

#include <cstdint>

#include "wav_player/wav_parser.h"

namespace crynsnd {

class WavPlayerPwm {
 public:
  explicit WavPlayerPwm(int pwm_gpio);
  ~WavPlayerPwm();

  void play(const crynsnd::WaveFile* file, bool loop = false);
  void stop();

  bool isPlaying();

 private:
  const int pwm_gpio_;
  int slice_num_;
};
}  // namespace crynsnd
#endif  // __crynsnd_wav_player_wav_player_pwm_h
