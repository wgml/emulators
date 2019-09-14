#pragma once
#include "emu/clock/freq.hpp"
#include "util/configuration.hpp"

#include <chrono>
#include <string>

namespace emu {
struct Clock
{
  using SysClock = std::chrono::high_resolution_clock;
  using Timepoint = std::chrono::time_point<SysClock, std::chrono::nanoseconds>;

  explicit Clock(Frequency f, std::string const& name, util::Configuration const& conf);

  void reset();

  void tick();

  std::size_t ticksFromStart() const;

  Frequency const freq;

private:
  Timepoint last;
  Timepoint start;
  std::size_t ticks;
  std::string const name;
  bool spin;
};
}  // namespace emu
