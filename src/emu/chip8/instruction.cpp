#include "emu/chip8/instruction.hpp"

#include "emu/chip8/machine.hpp"

#include <fmt/format.h>
#include <stdexcept>

namespace emu::chip8::op {
namespace instr {
auto noop()
{
  auto op = [](Machine&) {};

  return Instruction{false, std::move(op)};
}

auto cls()
{
  auto op = [](Machine& m) { m.clearDisplay(); };

  return Instruction{false, std::move(op)};
}

auto ret()
{
  auto op = [](Machine& m) {
    m.pc = m.stack[--m.sp];
    m.pc += 2;
  };

  return Instruction{true, std::move(op)};
}

auto jump(uint16_t arg)
{
  auto op = [arg](Machine& m) { m.pc = arg; };

  return Instruction{true, std::move(op)};
}

auto call(uint16_t arg)
{
  auto op = [arg](Machine& m) {
    m.stack[m.sp] = m.pc;
    ++m.sp;
    m.pc = arg;
  };

  return Instruction{true, std::move(op)};
}

auto skip_equal(uint16_t arg)
{
  uint8_t reg = (arg & 0xF00) >> 8;
  uint8_t value = arg & 0xFF;

  auto op = [reg, value](Machine& m) {
    if (m.V[reg] == value)
      m.pc += 2;
  };

  return Instruction(false, std::move(op));
}

auto skip_not_equal(uint16_t arg)
{
  uint8_t reg = (arg & 0xF00) >> 8;
  uint8_t value = arg & 0xFF;

  auto op = [reg, value](Machine& m) {
    if (m.V[reg] != value)
      m.pc += 2;
  };

  return Instruction{false, std::move(op)};
}

auto skip_reg_equal(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 8;

  auto op = [x, y](Machine& m) {
    if (m.V[x] == m.V[y])
      m.pc += 2;
  };

  return Instruction{false, std::move(op)};
}

auto set_reg(uint16_t arg)
{
  uint8_t reg = (arg & 0xF00) >> 8;
  uint8_t value = arg & 0xFF;

  auto op = [reg, value](Machine& m) { m.V[reg] = value; };

  return Instruction{false, std::move(op)};
}

auto add(uint16_t arg)
{
  uint8_t reg = (arg & 0xF00) >> 8;
  uint8_t value = arg & 0xFF;

  auto op = [reg, value](Machine& m) { m.V[reg] += value; };

  return Instruction{false, std::move(op)};
}

auto copy_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) { m.V[x] = m.V[y]; };

  return Instruction{false, std::move(op)};
}

auto or_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) { m.V[x] |= m.V[y]; };

  return Instruction{false, std::move(op)};
}

auto and_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) { m.V[x] &= m.V[y]; };

  return Instruction{false, std::move(op)};
}

auto xor_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) { m.V[x] ^= m.V[y]; };

  return Instruction{false, std::move(op)};
}

auto add_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) {
    if (m.V[x] > (0xFF - m.V[y]))
      m.V[0xF] = 1;

    m.V[x] += m.V[y];
  };

  return Instruction{false, std::move(op)};
}

auto sub_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) {
    if (m.V[x] > m.V[y])
      m.V[0xF] = 1;
    else
      m.V[0xF] = 0;

    m.V[x] -= m.V[y];
  };

  return Instruction{false, std::move(op)};
}

auto shift_right(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;

  auto op = [x](Machine& m) {
    m.V[0xF] = (m.V[x] & 1);

    m.V[x] >>= 1;
  };

  return Instruction{false, std::move(op)};
}

auto subn_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 4;

  auto op = [x, y](Machine& m) {
    m.V[0xF] = (m.V[y] > m.V[x]) ? 1 : 0;

    m.V[x] = m.V[y] - m.V[x];
  };

  return Instruction{false, std::move(op)};
}

auto shift_left(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;

  auto op = [x](Machine& m) {
    m.V[0xF] = (m.V[x] >> 7);

    m.V[x] <<= 1;
  };

  return Instruction{false, std::move(op)};
}

auto skip_reg_not_equal(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t y = (arg & 0x0F0) >> 8;

  auto op = [x, y](Machine& m) {
    if (m.V[x] != m.V[y])
      m.pc += 2;
  };

  return Instruction{false, std::move(op)};
}

auto set_I(uint16_t arg)
{
  auto op = [arg](Machine& m) { m.I = arg; };

  return Instruction{false, std::move(op)};
}

auto jump_reg(uint16_t arg)
{
  auto op = [arg](Machine& m) { m.pc = m.V[0] + arg; };

  return Instruction{true, std::move(op)};
}

auto rand(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  uint8_t value = arg & 0xFF;

  auto op = [x, value](Machine& m) {
    uint8_t r = (m.random())();

    m.V[x] = value & r;
  };

  return Instruction{false, std::move(op)};
}

auto draw(uint16_t arg)
{
  uint8_t Vx = (arg & 0xF00) >> 8;
  uint8_t Vy = (arg & 0x0F0) >> 4;
  uint8_t n = (arg & 0x00F);

  auto op = [Vx, Vy, n](Machine& m) {
    uint8_t x = m.V[Vx];
    uint8_t y = m.V[Vy];

    auto& VF = m.V[0xF];
    VF = 0;

    for (uint8_t row = 0; row < n; ++row)
    {
      uint8_t value = m.memory[m.I + row];
      for (uint8_t col = 0; col < 8; ++col)
      {
        uint8_t pixel = value & (0x80 >> col);
        if (!pixel)
          continue;

        auto displayPosition = col + x + (y + row) * 64;
        VF |= m.display[displayPosition];
        m.display[displayPosition] ^= 1;
      }
    }

    m.redraw = true;
  };

  return Instruction{false, std::move(op)};
}

auto skip_if_key(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    if (m.key[m.V[x]])
      m.pc += 2;
  };

  return Instruction{false, std::move(op)};
}

auto skip_if_not_key(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    if (!m.key[m.V[x]])
      m.pc += 2;
  };

  return Instruction{false, std::move(op)};
}

auto read_delay(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) { m.V[x] = m.delayTimer; };

  return Instruction{false, std::move(op)};
}

auto wait_on_key(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    for (std::size_t k = 0; k < 16; ++k)
    {
      if (m.key[k])
      {
        m.V[x] = k;
        m.pc += 2;
      }
    }
  };

  return Instruction{true, std::move(op)};
}

auto set_delay(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) { m.delayTimer = m.V[x]; };

  return Instruction{false, std::move(op)};
}

auto set_sound(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    m.soundTimer = m.V[x];
    logging::debug("Setting sound timer to {}", m.soundTimer);
  };

  return Instruction{false, std::move(op)};
}

auto set_I_reg(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) { m.I += m.V[x]; };

  return Instruction{false, std::move(op)};
}

auto set_I_digit(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) { m.I = m.V[x] * 5; };

  return Instruction{false, std::move(op)};
}

auto store_bcd(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    m.memory[m.I] = m.V[x] / 100;
    m.memory[m.I + 1] = (m.V[x] / 10) % 10;
    m.memory[m.I + 2] = m.V[x] % 10;
  };

  return Instruction{false, std::move(op)};
}

auto store_regs(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    for (std::size_t i = 0; i <= x; ++i)
      m.memory[m.I + i] = m.V[i];
  };

  return Instruction{false, std::move(op)};
}

auto load_regs(uint16_t arg)
{
  uint8_t x = (arg & 0xF00) >> 8;
  auto op = [x](Machine& m) {
    for (std::size_t i = 0; i <= x; ++i)
      m.V[i] = m.memory[m.I + i];
  };

  return Instruction{false, std::move(op)};
}
}  // namespace instr

namespace {
Instruction unsupported(uint16_t opcode)
{
  throw std::logic_error(fmt::format("Unsupported opcode: {:#0x}", opcode));
}
}  // namespace

Instruction parse(std::uint16_t opcode)
{
  if (opcode == 0)
    return instr::noop();

  logging::trace("parsing {:#0x}", opcode);
  uint16_t arg = opcode & 0x0FFF;

  switch (uint8_t op = (opcode & 0xF000) >> 12; op)
  {
    case 0x0:
      switch (arg)
      {
        case 0x0E0:
          return instr::cls();
        case 0x00EE:
          return instr::ret();
        default:
          return instr::noop();
      }
    case 0x1:
      return instr::jump(arg);
    case 0x2:
      return instr::call(arg);
    case 0x3:
      return instr::skip_equal(arg);
    case 0x4:
      return instr::skip_not_equal(arg);
    case 0x5:
      if ((arg & 0xF) != 0)
        return unsupported(opcode);
      return instr::skip_reg_equal(arg);
    case 0x6:
      return instr::set_reg(arg);
    case 0x7:
      return instr::add(arg);
    case 0x8:
      switch (auto type = (arg & 0xF); type)
      {
        case 0x0:
          return instr::copy_reg(arg);
        case 0x1:
          return instr::or_reg(arg);
        case 0x2:
          return instr::and_reg(arg);
        case 0x3:
          return instr::xor_reg(arg);
        case 0x4:
          return instr::add_reg(arg);
        case 0x5:
          return instr::sub_reg(arg);
        case 0x6:
          return instr::shift_right(arg);
        case 0x7:
          return instr::subn_reg(arg);
        case 0xE:
          return instr::shift_left(arg);
        default:
          return unsupported(opcode);
      }
    case 0x9:
      if ((arg & 0xF) != 0)
        return unsupported(opcode);
      return instr::skip_reg_not_equal(arg);
    case 0xA:
      return instr::set_I(arg);
    case 0xB:
      return instr::jump_reg(arg);
    case 0xC:
      return instr::rand(arg);
    case 0xD:
      return instr::draw(arg);
    case 0xE:
      switch (auto type = arg & 0xFF; type)
      {
        case 0x9E:
          return instr::skip_if_key(arg);
        case 0xA1:
          return instr::skip_if_not_key(arg);
        default:
          return unsupported(opcode);
      }
    case 0xF:
      switch (auto type = arg & 0xFF; type)
      {
        case 0x07:
          return instr::read_delay(arg);
        case 0x0A:
          return instr::wait_on_key(arg);
        case 0x15:
          return instr::set_delay(arg);
        case 0x18:
          return instr::set_sound(arg);
        case 0x1E:
          return instr::set_I_reg(arg);
        case 0x29:
          return instr::set_I_digit(arg);
        case 0x33:
          return instr::store_bcd(arg);
        case 0x55:
          return instr::store_regs(arg);
        case 0x65:
          return instr::load_regs(arg);
        default:
          return unsupported(opcode);
      }
    default:
      return unsupported(opcode);
  }

  return unsupported(opcode);
}
}  // namespace emu::chip8::op
