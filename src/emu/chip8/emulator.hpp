#pragma once
#include "emu/chip8/display.hpp"
#include "emu/chip8/input.hpp"
#include "emu/chip8/machine.hpp"
#include "emu/clock/clock.hpp"
#include "emu/random.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {

struct Emulator
{
  explicit Emulator(Ptr<Display> display, Ptr<Input> input, Ptr<Clock> clock, Program const& program);
  int operator()(bool sanitize);

  void stop()
  {
    stopFlag = true;
  }

private:
  Machine machine;
  Ptr<Display> display;
  Ptr<Input> input;
  Ptr<Clock> clock;
  Random<uint8_t> random;

  std::atomic_bool stopFlag = false;
};
}  // namespace emu::chip8
