#include "emu/clock/clock.hpp"

#include "util/log.hpp"

#include <thread>

namespace emu {
Clock::Clock(Frequency f, std::string const& name) : freq(f), name(name)
{
  reset();
}

void Clock::reset()
{
  last = SysClock::now();
  start = last;
  ticks = 0;
}

void Clock::tick()
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
    logging::debug("[{}] done {} ticks in {} seconds (freq is {}Hz)", name, ticks, runtime.count(), freq);
  }
}

std::size_t Clock::ticksFromStart() const
{
  return ticks;
}

}  // namespace emu
