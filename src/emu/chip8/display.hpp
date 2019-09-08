#pragma once
#include "emu/chip8/machine.hpp"
#include "io/display.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {

struct Display
{
  explicit Display(Ptr<io::ConsoleScreen> screen) : screen(screen)
  {}

  void draw(Machine const& machine);

private:
  Ptr<io::ConsoleScreen> screen;
};
}  // namespace emu::chip8
