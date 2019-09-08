#include "io/gui.hpp"

#include "io/render.hpp"
#include "util/log.hpp"

#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <stdexcept>

namespace io {

Display::Display(std::size_t consoleWidth, std::size_t consoleHeight) : consoleScreen(consoleWidth, consoleHeight)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    throw std::runtime_error(fmt::format("Failed to initialize graphics: {}", SDL_GetError()));

  window = SDL_CreateWindow("Emulate", dim.window.x, dim.window.y, dim.window.width, dim.window.height, 0);
  auto renderFlags = SDL_RENDERER_ACCELERATED;

  renderer = SDL_CreateRenderer(window, -1, renderFlags);
}

void Display::frame()
{
  pollEvents();

  SDL_SetRenderDrawColor(renderer, 20, 20, 20, 0);
  SDL_RenderClear(renderer);

  renderConsole();

  SDL_RenderPresent(renderer);
}

Display::~Display()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Display::renderConsole()
{
  SDL_Rect consoleBox = dim.console;
  SDL_Rect consoleWrap = dim.console.border(10);

  render::rectangle(renderer, consoleWrap, White);
  render::rectangle(renderer, consoleBox, Black);

  renderPixels();
}

void Display::renderPixels()
{
  auto pixelRows = consoleScreen.height;
  auto pixelCols = consoleScreen.width;

  auto pixelWidth = dim.console.width / pixelCols;
  auto pixelHeight = dim.console.height / pixelRows;
  auto constraint = std::min(pixelWidth, pixelHeight);
  pixelWidth = pixelHeight = constraint;
  auto adjustHorizontal = (dim.console.width - pixelWidth * pixelCols) / 2 + dim.console.x;
  auto adjustVertical = (dim.console.height - pixelHeight * pixelRows) / 2 + dim.console.y;

  for (std::size_t y = 0; y < pixelRows; ++y)
  {
    for (std::size_t x = 0; x < pixelCols; ++x)
    {
      SDL_Rect pixel;
      pixel.x = adjustHorizontal + x * pixelWidth;
      pixel.y = adjustVertical + y * pixelHeight;
      pixel.w = pixelWidth;
      pixel.h = pixelHeight;

      auto color = consoleScreen(y, x);
      render::rectangle(renderer, pixel, color);
    }
  }
}

void Display::pollEvents()
{
  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_KEYDOWN:
        [[fallthrough]];
      case SDL_KEYUP:
      {
        auto key = event.key;
        bool down = key.type == SDL_KEYDOWN;

        if (key.keysym.sym == SDLK_ESCAPE && down)
          quit = true;
        else if (down)
          keys.pressed(key.keysym.sym);
        else
          keys.released(key.keysym.sym);

        std::string name = SDL_GetKeyName(key.keysym.sym);
        logging::debug("Key event: k={}, down={}", name, down);
        break;
      }
      case SDL_QUIT:
        quit = true;
        break;
      default:
        break;
    }
  }
}
}  // namespace io
