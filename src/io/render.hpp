#pragma once
#include "io/video.hpp"
#include "util/ptr.hpp"

#include <SDL2/SDL.h>

namespace io::render {
void rectangle(Ptr<SDL_Renderer> renderer, SDL_Rect r, Color c);
}
