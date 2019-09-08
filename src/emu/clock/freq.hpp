#pragma once
#include <chrono>

namespace emu {
struct Frequency
{
  using Nanos = std::chrono::nanoseconds;

  explicit constexpr Frequency(uint64_t h) : hertz(h)
  {}

  constexpr Nanos interval() const
  {
    return Nanos{1'000'000'000 / hertz};
  }

  uint64_t hertz;
};
}  // namespace emu

constexpr emu::Frequency operator""_Hz(unsigned long long hz)
{
  return emu::Frequency(hz);
}
constexpr emu::Frequency operator""_kHz(long double kHz)
{
  return emu::Frequency(kHz * 1000);
}

