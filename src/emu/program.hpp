#pragma once
#include "util/log.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

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

  static Program read(std::string const& filename)
  {
    logging::debug("Reading content of {}", filename);
    if (!std::filesystem::exists(filename))
      throw std::logic_error("Input file does not exist");

    std::ifstream in(filename, std::ios::binary);

    return Program{std::vector<uint8_t>(std::istreambuf_iterator<char>(in), {})};
  }
};
}  // namespace emu
