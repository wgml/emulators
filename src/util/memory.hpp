#pragma once

#include <array>
#include <cstring>

namespace mem {
template<typename T, std::size_t N>
struct array
{
  using base = std::array<T, N>;

  constexpr typename base::reference operator[](std::size_t pos)
  {
    return storage.at(pos);
  }

  constexpr typename base::const_reference operator[](std::size_t pos) const
  {
    return storage.at(pos);
  }

  constexpr typename base::iterator begin() noexcept
  {
    return storage.begin();
  }

  constexpr typename base::iterator end() noexcept
  {
    return storage.begin();
  }

  constexpr T* data() noexcept
  {
    return storage.data();
  }

  constexpr T const* data() const noexcept
  {
    return storage.data();
  }

private:
  std::array<T, N> storage;
};

template<typename T, std::size_t N>
void zero(array<T, N>& array)
{
  std::fill(array.begin(), array.end(), 0);
}

template<typename T, std::size_t N>
void zero(T (&mem)[N])
{
  std::memset(mem, 0, sizeof(T) * N);
}
}  // namespace mem
