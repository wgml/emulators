#include "emu/chip8/emulator.hpp"

namespace emu::chip8 {
Emulator::Emulator(Ptr<Display> display, Ptr<Input> input, Ptr<Clock> clock, Program const& program)
  : display(display), input(input), clock(clock)
{
  machine.reset(program);
}

int Emulator::operator()(bool sanitize)
{
  clock->reset();
  while (true)
  {
    clock->tick();

    if (auto res = machine.cycle(sanitize); !res && sanitize)
      break;

    if (std::exchange(machine.redraw, false))
      display->draw(machine);

    if (std::exchange(machine.sound, false))
      logging::info("beep");

    machine.input(input);
  }
  return 0;
}

}  // namespace emu::chip8

