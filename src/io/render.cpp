#include "io/render.hpp"

namespace io::render {

void rectangle(Ptr<SDL_Renderer> renderer, SDL_Rect r, Color c)
{
  SDL_SetRenderDrawColor(renderer.get(), c.r, c.g, c.b, 0);
  SDL_RenderFillRect(renderer.get(), &r);
}
}  // namespace io::render
