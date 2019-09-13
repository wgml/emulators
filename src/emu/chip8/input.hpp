#pragma once
#include "emu/chip8/machine.hpp"
#include "io/keyboard.hpp"
#include "util/ptr.hpp"

namespace emu::chip8 {
struct Input
{
  explicit Input(Ptr<io::KeysInput> keys) : keys(keys)
  {
    keys->onRelease(io::Keys::P, [this] { pause.update(); });
  }

  void update(Machine& machine);

  bool paused() const
  {
    return pause.active;
  }

private:
  Ptr<io::KeysInput> keys;

  struct PauseControl
  {
    bool active = false;
    void update();
  };
  PauseControl pause;
};
}  // namespace emu::chip8
