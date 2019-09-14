#pragma once
#include "emu/chip8/instruction.hpp"
#include "emu/chip8/machine_fwd.hpp"
#include "emu/program.hpp"
#include "emu/random.hpp"
#include "util/debug.hpp"
#include "util/log.hpp"
#include "util/ptr.hpp"

#include <algorithm>

namespace emu::chip8 {
struct Machine
{
  mem::array<uint8_t, 4096> memory;
  mem::array<op::Instruction, (4096 - 512) / 2> instructions;

  mem::array<uint8_t, 16> V;

  uint16_t I;
  uint16_t pc;

  mem::array<uint8_t, 64 * 32> display;

  uint8_t delayTimer;
  uint8_t soundTimer;

  mem::array<uint16_t, 16> stack;
  uint16_t sp;

  mem::array<uint8_t, 16> key;

  bool redraw = false;
  bool sound = true;

  bool cycle(bool sanitize);
  void reset(emu::Program const& program, Ptr<Random<uint8_t>> randomDevice);
  void clearDisplay();

  Random<uint8_t>& random()
  {
    return *randomDevice;
  }

private:
  void parseInstructions();

  void execute();

  Ptr<Random<uint8_t>> randomDevice;

  constexpr static unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
  };
};

}  // namespace emu::chip8
