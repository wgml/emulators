#pragma once

#include <SDL2/SDL_keycode.h>
#include <set>

namespace io {
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
}  // namespace io
