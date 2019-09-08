#pragma once
#include <cstddef>
#include <string>

namespace util {

struct Configuration
{
  bool verbose = false;
  bool trace = false;

  std::string rom;
};

Configuration parse_args(int argc, char* argv[]);
}  // namespace util
