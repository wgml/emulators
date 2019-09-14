#pragma once

#include "emu/chip8/machine_fwd.hpp"

#include <cstdint>
#include <functional>
#include <utility>

namespace emu::chip8::op {
struct Instruction
{
  using Op = std::function<void(Machine&)>;

  explicit Instruction() : op(nullptr), jump(false)
  {}

  explicit Instruction(bool jump, Op op) : op(std::move(op)), jump(jump)
  {}

  Op op;
  bool jump;
};

Instruction parse(std::uint16_t arg);
}  // namespace emu::chip8::op
