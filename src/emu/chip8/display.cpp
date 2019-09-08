#include "emu/chip8/display.hpp"

#include "util/log.hpp"

namespace emu::chip8 {

void Display::draw(Machine const& machine)
{
  std::stringstream result;

  for (std::size_t i = 0; i < 64 * 32; ++i)
  {
    if (i % 64 == 0)
      result << '\n';
    result << (machine.display[i] == 1 ? "█" : " ");
  }

  logging::info(result.str());
}
}  // namespace emu::chip8
