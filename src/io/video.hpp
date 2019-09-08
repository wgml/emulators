#pragma once

#include <SDL2/SDL.h>
#include <vector>

namespace io {

struct Pixel
{
  Pixel& operator=(bool set)
  {
    r = g = b = (set ? 255 : 0);
    return *this;
  }

  operator SDL_Color() const
  {
    return SDL_Color{r, g, b, 0};
  }

  uint8_t r, g, b;
};

using Color = Pixel;

namespace colors {
constexpr Color const White = Color{255, 255, 255};
constexpr Color const Red = Color{255, 0, 0};
constexpr Color const Green = Color{0, 255, 0};
constexpr Color const Blue = Color{0, 0, 255};
constexpr Color const Black = Color{0, 0, 0};
constexpr Color const Gray = Color{105, 105, 105};
}  // namespace colors

struct ConsoleScreen
{
  explicit ConsoleScreen(std::size_t width, std::size_t height);

  Pixel& operator()(std::size_t row, std::size_t col);

  std::size_t const width;
  std::size_t const height;

private:
  std::vector<Pixel> pixels;
};

}  // namespace io
