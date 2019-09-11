#pragma once

#include "io/audio.hpp"
#include "io/keyboard.hpp"
#include "io/video.hpp"
#include "util/ptr.hpp"

#include <SDL2/SDL.h>

namespace io {

struct Display
{
  explicit Display(std::size_t consoleWidth, std::size_t consoleHeight);
  ~Display();

  void frame();

  bool closed() const
  {
    return quit;
  }

  Ptr<ConsoleScreen> screen()
  {
    return &consoleScreen;
  }

  Ptr<KeysInput> input()
  {
    return &keys;
  }

  Ptr<Audio> audio()
  {
    return &consoleAudio;
  }

private:
  void renderConsole();

  void renderPixels();

  void pollEvents();

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  ConsoleScreen consoleScreen;
  KeysInput keys;
  Audio consoleAudio;

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
