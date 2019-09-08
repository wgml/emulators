#include "io/video.hpp"

#include <SDL2/SDL.h>
#include <vector>

namespace io {

ConsoleScreen::ConsoleScreen(std::size_t width, std::size_t height) : width(width), height(height)
{
  pixels.resize(width * height);
}

Pixel& ConsoleScreen::operator()(std::size_t row, std::size_t col)
{
  auto index = width * row + col;
  return pixels[index];
}
}  // namespace io
