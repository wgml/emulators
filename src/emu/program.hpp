#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace emu {
struct Program
{
  std::vector<uint8_t> content;

  auto begin() const
  {
    return std::begin(content);
  }

  auto end() const
  {
    return std::end(content);
  }

  auto size() const
  {
    return std::size(content);
  }

  static Program read(std::string const& filename);
};
}  // namespace emu
