#pragma once
#include "util/memory.hpp"

#include <iomanip>
#include <sstream>

namespace debug {

std::string dump_memory(uint8_t const* mem, std::size_t length, std::size_t offset = 0);

template<std::size_t N>
std::string dump_memory(mem::array<uint8_t, N> const& array, std::size_t offset = 0)
{
  return dump_memory(array.data(), N, offset);
}
}  // namespace debug
