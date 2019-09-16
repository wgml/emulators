#pragma once
#include "io/video.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {

struct Display
{
  explicit Display(Ptr<io::ConsoleScreen> screen) : screen(screen)
  {}

  void clear();

  bool flip(std::size_t row, std::size_t col);

private:
  Ptr<io::ConsoleScreen> screen;
};
}  // namespace emu::chip8
