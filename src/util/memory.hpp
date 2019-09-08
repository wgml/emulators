#pragma once

#include <cstring>

namespace mem {
template<typename T, std::size_t N>
void zero(T (&mem)[N])
{
  std::memset(mem, 0, sizeof(T) * N);
}
}  // namespace mem
