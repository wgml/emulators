#pragma once
#include "emu/chip8/machine.hpp"

namespace emu::chip8 {

struct Display
{
  void draw(Machine const& machine);
};
}  // namespace emu::chip8
