#pragma once

#include "emu/chip8/machine.hpp"
#include "io/audio.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {
struct Audio
{
  explicit Audio(Ptr<io::Audio>);

  void update(Machine const& machine);

private:
  bool beeping = false;
  Ptr<io::Audio> audio;
};
}  // namespace emu::chip8
