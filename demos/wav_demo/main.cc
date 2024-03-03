#include <stdio.h>
#include <string.h>

#include "adafruit_macropad/adafruit_macropad.h"
#include "extra_data.h"
#include "pico/stdlib.h"
#include "wav_player/wav_parser.h"
#include "wav_player/wav_player_pwm.h"

int main() {
  // This clock speed makes it easier to set the correct frequency for the PWM
  set_sys_clock_khz(176000, true);

  crynsnd::AdafruitMacropad macropad;
  macropad.init();

  const crynsnd::DataDescriptor wav_files[] = {
      crynsnd::GetDataDescriptor("file1.wav"),
      crynsnd::GetDataDescriptor("file2.wav"),
      crynsnd::GetDataDescriptor("file3.wav"),
  };
  std::unique_ptr<crynsnd::WaveFile> wav_samples[] = {
      crynsnd::WaveFile::Create(wav_files[0].address),
      crynsnd::WaveFile::Create(wav_files[1].address),
      crynsnd::WaveFile::Create(wav_files[2].address),
  };

  crynsnd::WavPlayerPwm wave_player(MACROPAD_SDA);

  crynsnd::DisplayManager& display = macropad.getDisplay();
  while (true) {
    sleep_ms(33);
    display.clear(/*commit=*/false);
    if (wave_player.isPlaying()) {
      display.write(0, 1, "PLAYING!", /*commit=*/true);
      continue;
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_1)) {
      display.write(0, 3, "KEY 1", /*commit=*/false);
      wave_player.play(wav_samples[0].get());
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_2)) {
      display.write(45, 3, "KEY 2", /*commit=*/false);
       wave_player.play(wav_samples[1].get());
    }
    if (macropad.isKeyPressed(MACROPAD_KEY_3)) {
      display.write(90, 3, "KEY 3", /*commit=*/false);
      wave_player.play(wav_samples[2].get());
    }
    display.flush();
  }
}
