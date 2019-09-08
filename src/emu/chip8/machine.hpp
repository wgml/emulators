#pragma once
#include "emu/program.hpp"
#include "emu/random.hpp"
#include "util/debug.hpp"
#include "util/log.hpp"
#include "util/ptr.hpp"

#include <algorithm>

namespace emu::chip8 {
struct Machine
{
  uint8_t memory[4096];

  uint8_t V[16];

  uint16_t I;
  uint16_t pc;

  uint8_t display[64 * 32];

  uint8_t delayTimer;
  uint8_t soundTimer;

  uint16_t stack[16];
  uint16_t sp;

  uint8_t key[16];

  bool redraw = false;
  bool sound = true;

  bool cycle(bool sanitize);
  void reset(emu::Program const& program, Ptr<Random<uint8_t>> randomDevice);

private:
  uint16_t opcode() const;
  void clearDisplay();
  bool unsupported(uint16_t opcode);
  bool wait();
  bool execute(uint16_t opcode);
  void dump_state() const;

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
