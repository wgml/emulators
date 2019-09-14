#include "emu/clock/clock.hpp"

#include "util/log.hpp"

#include <thread>

namespace emu {
Clock::Clock(Frequency f, std::string const& name, util::Configuration const& conf)
  : freq(f), name(name), spin(conf.emulation.spin)
{
  reset();
  logging::info("[{}] Running with {}tps.", name, freq.hertz);
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
  auto const now = SysClock::now();
  if (nextTick < now)
    logging::trace(
      "Next tick is in the past, lagging {}ns behind",
      std::chrono::duration_cast<std::chrono::nanoseconds>(last - nextTick).count());

  if (spin)
    while (SysClock::now() < nextTick)
    {
    }
  else if (nextTick > now)
    std::this_thread::sleep_until(nextTick);

  last = SysClock::now();
  ++ticks;

  if (ticks % freq.hertz == 0)
  {
    auto runtime = std::chrono::duration_cast<std::chrono::duration<double>>(last - start);
    double freq = ticks / runtime.count();
    logging::info("[{}] done {} ticks in {} seconds (freq is {:f}Hz)", name, ticks, runtime.count(), freq);
  }
}  // namespace emu

std::size_t Clock::ticksFromStart() const
{
  return ticks;
}

}  // namespace emu
