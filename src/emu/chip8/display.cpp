#include "emu/chip8/display.hpp"

#include "util/log.hpp"

namespace emu::chip8 {

void Display::draw(Machine const& machine)
{
  for (std::size_t y = 0; y < 32; ++y)
    for (std::size_t x = 0; x < 64; ++x)
      (*screen)(y, x) = machine.display[64 * y + x] == 1;
}
}  // namespace emu::chip8
