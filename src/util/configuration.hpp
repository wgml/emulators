#pragma once
#include <cstddef>
#include <string>

namespace util {

struct Configuration
{
  bool verbose = false;
  bool trace = false;

  struct
  {
    bool sanitize = false;
    std::string rom;
    bool spin = false;
  } emulation;
};

Configuration parse_args(int argc, char* argv[]);
}  // namespace util
