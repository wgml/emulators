#pragma once

#include "emu/clock/freq.hpp"

namespace io {

struct Audio
{
  explicit Audio();
  ~Audio();

  void beep(emu::Frequency freq);
  void endBeep();

  void generate(std::int16_t* stream, int length);

private:
  bool active;
  emu::Frequency currentFreq;
  int sampleNumber = 0;

  constexpr static const int amp = 8000;
  constexpr static const emu::Frequency freq = 44100_Hz;
};

}  // namespace io
