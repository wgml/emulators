#include "emu/chip8/display.hpp"

#include "util/log.hpp"

namespace emu::chip8 {

namespace {
bool isOn(io::Pixel const& pixel)
{
  return pixel.r > 0;
}
}  // namespace

void Display::clear()
{
  for (std::size_t y = 0; y < 32; ++y)
    for (std::size_t x = 0; x < 64; ++x)
      (*screen)(y, x) = false;
}

bool Display::flip(std::size_t row, std::size_t col)
{
  auto& pixel = (*screen)(row, col);
  auto prev = isOn(pixel);
  pixel = !prev;

  return prev;
}
}  // namespace emu::chip8
