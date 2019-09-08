#pragma once
#include "io/display.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {
struct Input
{
  explicit Input(Ptr<io::KeysInput> keys) : keys(keys)
  {}

  void update(uint8_t (&key)[16]);

private:
  Ptr<io::KeysInput> keys;
};
}  // namespace emu::chip8
