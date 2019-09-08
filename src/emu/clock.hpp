#pragma once
#include "util/log.hpp"

#include <chrono>
#include <thread>

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

namespace emu {
struct Clock
{
  using SysClock = std::chrono::system_clock;
  using Timepoint = std::chrono::time_point<SysClock>;

  explicit Clock(Frequency f) : freq(f)
  {
    reset();
  }

  void reset()
  {
    last = SysClock::now();
    start = last;
    ticks = 0;
  }

  void tick()
  {
    auto nextTick = last + freq.interval();
    if (nextTick < last)
      logging::warn(
        "Next tick is in the past, lagging {}ns behind",
        std::chrono::duration_cast<std::chrono::nanoseconds>(last - nextTick).count());
    else
      std::this_thread::sleep_until(nextTick);
    last = nextTick;
    ++ticks;

    if (ticks % freq.hertz == 0)
    {
      auto runtime = std::chrono::duration_cast<std::chrono::duration<double>>(last - start);
      double freq = ticks / runtime.count();
      logging::debug("Done {} ticks in {} seconds (freq is {}Hz)", ticks, runtime.count(), freq);
    }
  }

  std::size_t ticksFromStart() const
  {
    return ticks;
  }

  Frequency const freq;

private:
  Timepoint last;
  Timepoint start;
  std::size_t ticks;
};
}  // namespace emu
