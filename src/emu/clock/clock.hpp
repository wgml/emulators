#pragma once
#include "emu/clock/freq.hpp"

#include <chrono>
#include <string>

namespace emu {
struct Clock
{
  using SysClock = std::chrono::system_clock;
  using Timepoint = std::chrono::time_point<SysClock>;

  explicit Clock(Frequency f, std::string const& name);

  void reset();

  void tick();

  std::size_t ticksFromStart() const;

  Frequency const freq;

private:
  Timepoint last;
  Timepoint start;
  std::size_t ticks;
  std::string const name;
};
}  // namespace emu
