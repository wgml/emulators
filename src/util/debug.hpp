#pragma once
#include "util/memory.hpp"

#include <iomanip>
#include <sstream>

namespace debug {

std::string dump_memory(uint8_t const* mem, std::size_t length, std::size_t offset = 0);

template<typename T, std::size_t N>
std::string dump_memory(mem::array<T, N> const& array, std::size_t offset = 0)
{
  return dump_memory((uint8_t const*)array.data(), N * sizeof(T), offset * sizeof(T));
}
}  // namespace debug
