#pragma once
#include "emu/chip8/display.hpp"
#include "emu/chip8/input.hpp"
#include "emu/chip8/machine.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {

struct Emulator
{
  explicit Emulator(Ptr<Display> display, Ptr<Input> input, Ptr<Clock> clock, Program const& program);
  int operator()(bool sanitize);

private:
  Machine machine;
  Ptr<Display> display;
  Ptr<Input> input;
  Ptr<Clock> clock;
};
}  // namespace emu::chip8
