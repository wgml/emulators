#pragma once
#include <random>

namespace emu {
template<typename Int>
struct Random
{
  explicit Random() : rd(), gen(rd()), dist(std::numeric_limits<Int>::min(), std::numeric_limits<Int>::max())
  {}

  Int operator()()
  {
    return dist(gen);
  }

private:
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<Int> dist;
};
}  // namespace emu
