#pragma once
#include "util/log.hpp"
#include "util/ptr.hpp"

#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <set>
#include <stdexcept>

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

constexpr Color const White = Color{255, 255, 255};
constexpr Color const Black = Color{0, 0, 0};

struct ConsoleScreen
{
  explicit ConsoleScreen(std::size_t width, std::size_t height) : width(width), height(height)
  {
    pixels.resize(width * height);
  }

  Pixel& operator()(std::size_t row, std::size_t col)
  {
    auto index = width * row + col;
    return pixels[index];
  }

  std::size_t const width;
  std::size_t const height;

private:
  std::vector<Pixel> pixels;
};

enum class Keys : SDL_Keycode
{
  Num0 = SDLK_0,
  Num1 = SDLK_1,
  Num2 = SDLK_2,
  Num3 = SDLK_3,
  Num4 = SDLK_4,
  Num5 = SDLK_5,
  Num6 = SDLK_6,
  Num7 = SDLK_7,
  Num8 = SDLK_8,
  Num9 = SDLK_9,
  Q = SDLK_q,
  W = SDLK_w,
  E = SDLK_e,
  R = SDLK_r,
  T = SDLK_t,
  Y = SDLK_y,
  U = SDLK_u,
  I = SDLK_i,
  O = SDLK_o,
  P = SDLK_p,
  A = SDLK_a,
  S = SDLK_s,
  D = SDLK_d,
  F = SDLK_f,
  G = SDLK_g,
  H = SDLK_h,
  J = SDLK_j,
  K = SDLK_k,
  L = SDLK_l,
  Z = SDLK_z,
  X = SDLK_x,
  C = SDLK_c,
  V = SDLK_v,
  B = SDLK_b,
  N = SDLK_n,
  M = SDLK_m
};

struct KeysInput
{
  bool isPressed(Keys key)
  {
    return active.count(static_cast<SDL_Keycode>(key)) != 0;
  }

  void pressed(SDL_Keycode const& key)
  {
    active.insert(key);
  }

  void released(SDL_Keycode const& key)
  {
    active.erase(key);
  }

private:
  std::set<SDL_Keycode> active;
};

struct Display
{
  explicit Display(std::size_t consoleWidth, std::size_t consoleHeight) : consoleScreen(consoleWidth, consoleHeight)
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
      throw std::runtime_error(fmt::format("Failed to initialize graphics: {}", SDL_GetError()));

    window = SDL_CreateWindow("Emulate", dim.window.x, dim.window.y, dim.window.width, dim.window.height, 0);
    auto renderFlags = SDL_RENDERER_ACCELERATED;

    renderer = SDL_CreateRenderer(window, -1, renderFlags);
  }

  void frame()
  {
    pollEvents();

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 0);
    SDL_RenderClear(renderer);

    renderConsole();

    SDL_RenderPresent(renderer);
  }

  bool closed() const
  {
    return quit;
  }

  ~Display()
  {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }

  Ptr<ConsoleScreen> screen()
  {
    return &consoleScreen;
  }

  Ptr<KeysInput> input()
  {
    return &keys;
  }

private:
  void renderConsole()
  {
    SDL_Rect consoleBox = dim.console;
    SDL_Rect consoleWrap = dim.console.border(10);

    renderRectangle(consoleWrap, White);
    renderRectangle(consoleBox, Black);

    renderPixels();
  }

  void renderPixels()
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
        renderRectangle(pixel, color);
      }
    }
  }

  void renderRectangle(SDL_Rect r, Color c)
  {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0);
    SDL_RenderFillRect(renderer, &r);
  }

  void pollEvents()
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

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  ConsoleScreen consoleScreen;
  KeysInput keys;

  struct
  {
    struct Box
    {
      std::size_t x, y, width, height;

      operator SDL_Rect() const
      {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = width;
        r.h = height;

        return r;
      }

      Box border(std::size_t b) const
      {
        return Box{x - b, y - b, width + 2 * b, height + 2 * b};
      }
    };

    Box window{100, 100, 1000, 800};
    Box console{50, 50, 600, 600};
  } dim;

  bool quit = false;
};
}  // namespace io
