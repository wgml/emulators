#pragma once
#include "emu/chip8/machine.hpp"
#include "io/keyboard.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {
struct Input
{
  explicit Input(Ptr<io::KeysInput> keys) : keys(keys)
  {}

  void update(Machine& machine);

private:
  Ptr<io::KeysInput> keys;
};
}  // namespace emu::chip8
