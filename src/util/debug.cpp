#include "util/debug.hpp"

namespace debug {

namespace {
constexpr char escape = 27;

std::ostream& bold(std::ostream& os)
{
  return os << escape << "[1m";
}

std::ostream& normal(std::ostream& os)
{
  return os << escape << "[0m";
}
}  // namespace

std::string dump_memory(uint8_t const* mem, std::size_t length, std::size_t offset)
{
  std::stringstream result;
  result << std::setfill('0');

  for (std::size_t i = 0; i < length; ++i)
  {
    if (i % 16 == 0)
      result << bold << std::setw(4) << std::hex << i + offset << normal << ' ' << std::setw(2);
    else if (i % 4 == 0)
      result << ' ';

    result << std::setw(2) << std::hex << (int)mem[i] << ' ';

    if (i % 16 == 15 && i != length - 1)
      result << '\n';
  }

  return std::move(result).str();
}
}  // namespace debug
