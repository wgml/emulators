#include "emu/chip8/emulator.hpp"

namespace emu::chip8 {
Emulator::Emulator(Ptr<Display> display, Ptr<Audio> audio, Ptr<Input> input, Ptr<Clock> clock, Program const& program)
  : display(display), audio(audio), input(input), clock(clock)
{
  machine.reset(program, &random);
}

int Emulator::operator()(bool sanitize)
{
  clock->reset();
  while (!stopFlag)
  {
    clock->tick();

    if (auto res = machine.cycle(sanitize); !res && sanitize)
      break;

    if (std::exchange(machine.redraw, false))
      display->draw(machine);

    audio->update(machine);
    input->update(machine);
  }

  stopFlag = true;
  logging::info("Emulation stopped after {} frames.", clock->ticksFromStart());
  return 0;
}

}  // namespace emu::chip8

