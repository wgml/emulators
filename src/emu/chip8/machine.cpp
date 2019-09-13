#include "emu/chip8/machine.hpp"

#include "emu/clock/clock.hpp"
#include "emu/program.hpp"
#include "util/debug.hpp"
#include "util/log.hpp"
#include "util/memory.hpp"
#include "util/ptr.hpp"

#include <algorithm>

namespace emu::chip8 {
void Machine::reset(emu::Program const& program, Ptr<Random<uint8_t>> rd)
{
  pc = 0x200;
  I = 0;
  sp = 0;

  redraw = true;

  randomDevice = rd;

  mem::zero(memory);
  mem::zero(V);
  mem::zero(display);
  mem::zero(stack);
  mem::zero(key);

  std::copy(std::begin(fontset), std::end(fontset), memory.begin());
  std::copy(program.begin(), program.end(), std::next(memory.begin(), 512));
  logging::debug("Program is: \n{}", debug::dump_memory(program.content.data(), program.size()));
}

bool Machine::cycle(bool sanitize)
{
  uint16_t op = opcode();
  if (auto res = execute(op); !res && sanitize)
    return false;

  if (delayTimer > 0)
    --delayTimer;

  if (soundTimer > 0)
    --soundTimer;
  sound = soundTimer != 0;

  return true;
}

uint16_t Machine::opcode() const
{
  return memory[pc] << 8 | memory[pc + 1];
}

void Machine::clearDisplay()
{
  mem::zero(display);
}

bool Machine::unsupported(uint16_t opcode)
{
  logging::warn("Unsupported opcode: {:x}", opcode);
  return false;
}

bool Machine::wait()
{
  return true;
}

bool Machine::execute(uint16_t opcode)
{
  uint8_t op = (opcode & 0xF000) >> 12;
  uint16_t arg = opcode & 0x0FFF;

  logging::trace("pc={:x}, opcode={:>04X},op={:0X}, arg={:>03X}", pc, opcode, op, arg);

  switch (op)
  {
    case 0x0:
    {
      switch (arg)
      {
        case 0x0E0:
          clearDisplay();
          break;
        case 0x00EE:
          pc = stack[--sp];
          break;
        default:
          break;
      }
      pc += 2;
      break;
    }

    case 0x1:
    {
      pc = arg;
      break;
    }

    case 0x2:
    {
      stack[sp] = pc;
      ++sp;
      pc = arg;
      break;
    }

    case 0x3:
    {
      uint8_t reg = (arg & 0xF00) >> 8;
      uint8_t value = arg & 0xFF;

      if (V[reg] == value)
        pc += 2;

      pc += 2;
      break;
    }

    case 0x4:
    {
      uint8_t reg = (arg & 0xF00) >> 8;
      uint8_t value = arg & 0xFF;

      if (V[reg] != value)
        pc += 2;

      pc += 2;
      break;
    }

    case 0x5:
    {
      if ((arg & 0xF) != 0)
        return unsupported(opcode);

      uint8_t x = (arg & 0xF00) >> 8;
      uint8_t y = (arg & 0x0F0) >> 8;

      if (V[x] == V[y])
        pc += 2;

      pc += 2;
      break;
    }

    case 0x6:
    {
      uint8_t reg = (arg & 0xF00) >> 8;
      uint8_t value = arg & 0xFF;

      V[reg] = value;

      pc += 2;
      break;
    }

    case 0x7:
    {
      uint8_t reg = (arg & 0xF00) >> 8;
      uint8_t value = arg & 0xFF;

      V[reg] += value;

      pc += 2;
      break;
    }

    case 0x8:
    {
      uint8_t x = (arg & 0xF00) >> 8;
      uint8_t y = (arg & 0x0F0) >> 4;
      uint8_t type = (arg & 0xF);

      switch (type)
      {
        case 0x0:
        {
          V[x] = V[y];
          break;
        }
        case 0x1:
        {
          V[x] |= V[y];
          break;
        }
        case 0x2:
        {
          V[x] &= V[y];
          break;
        }
        case 0x3:
        {
          V[x] ^= V[y];
          break;
        }
        case 0x4:
        {
          if (V[x] > (0xFF - V[y]))
            V[0xF] = 1;

          V[x] += V[y];
          break;
        }
        case 0x5:
        {
          if (V[x] > V[y])
            V[0xF] = 1;
          else
            V[0xF] = 0;

          V[x] -= V[y];
          break;
        }
        case 0x6:
        {
          V[0xF] = (V[x] & 1);

          V[x] >>= 1;
          break;
        }
        case 0x7:
        {
          V[0xF] = (V[y] > V[x]) ? 1 : 0;

          V[x] = V[y] - V[x];
          break;
        }
        case 0xE:
        {
          V[0xF] = (V[x] >> 7);

          V[x] <<= 1;
          break;
        }
        default:
          return unsupported(opcode);
      }

      pc += 2;
      break;
    }

    case 0x9:
    {
      if ((arg & 0xF) != 0)
        return unsupported(opcode);

      uint8_t x = (arg & 0xF00) >> 8;
      uint8_t y = (arg & 0x0F0) >> 8;

      if (V[x] != V[y])
        pc += 2;

      pc += 2;
      break;
    }

    case 0xA:
    {
      I = arg;
      pc += 2;
      break;
    }

    case 0xB:
    {
      pc = V[0] + arg;
      break;
    }

    case 0xC:
    {
      uint8_t x = (arg & 0xF00) >> 8;
      uint8_t value = arg & 0xFF;
      uint8_t r = (*randomDevice)();

      V[x] = value & r;

      pc += 2;
      break;
    }

    case 0xD:
    {
      // dump_state();
      uint8_t Vx = (arg & 0xF00) >> 8;
      uint8_t Vy = (arg & 0x0F0) >> 4;
      uint8_t x = V[Vx];
      uint8_t y = V[Vy];
      uint8_t n = (arg & 0x00F);

      auto& VF = V[0xF];
      VF = 0;

      for (uint8_t row = 0; row < n; ++row)
      {
        uint8_t value = memory[I + row];
        for (uint8_t col = 0; col < 8; ++col)
        {
          uint8_t pixel = value & (0x80 >> col);
          if (!pixel)
            continue;

          auto displayPosition = col + x + (y + row) * 64;
          VF |= display[displayPosition];
          display[displayPosition] ^= 1;
        }
      }

      redraw = true;
      pc += 2;
      break;
    }

    case 0xE:
    {
      uint8_t x = (arg & 0xF00) >> 8;
      uint8_t type = arg & 0xFF;

      switch (type)
      {
        case 0x9E:
        {
          if (key[V[x]])
            pc += 2;

          break;
        }
        case 0xA1:
        {
          if (!key[V[x]])
            pc += 2;

          break;
        }
        default:
          return unsupported(opcode);
      }

      pc += 2;
      break;
    }
    case 0xF:
    {
      uint8_t x = (arg & 0xF00) >> 8;
      uint8_t type = arg & 0xFF;

      switch (type)
      {
        case 0x07:
        {
          V[x] = delayTimer;
          break;
        }
        case 0x0A:
        {
          for (std::size_t k = 0; k < 16; ++k)
          {
            if (key[k])
            {
              V[x] = k;
              break;
            }
          }

          return wait();
        }
        case 0x15:
        {
          delayTimer = V[x];
          break;
        }
        case 0x18:
        {
          soundTimer = V[x];
          logging::debug("Setting sound timer to {}", soundTimer);
          break;
        }
        case 0x1E:
        {
          I += V[x];
          break;
        }
        case 0x29:
        {
          I = V[x] * 5;
          break;
        }
        case 0x33:
        {
          memory[I] = V[x] / 100;
          memory[I + 1] = (V[x] / 10) % 10;
          memory[I + 2] = V[x] % 10;
          break;
        }
        case 0x55:
        {
          for (std::size_t i = 0; i <= x; ++i)
            memory[I + i] = V[i];
          break;
        }
        case 0x65:
        {
          for (std::size_t i = 0; i <= x; ++i)
            V[i] = memory[I + i];
          break;
        }
        default:
          return unsupported(opcode);
      }

      pc += 2;
      break;
    }

    default:
    {
      return unsupported(opcode);
    }
  }

  return true;
}

void Machine::dump_state() const
{
  printf("------------------------------------------------------------------\n");
  printf("\n");

  printf("V0: 0x%02x  V4: 0x%02x  V8: 0x%02x  VC: 0x%02x\n", V[0], V[4], V[8], V[12]);
  printf("V1: 0x%02x  V5: 0x%02x  V9: 0x%02x  VD: 0x%02x\n", V[1], V[5], V[9], V[13]);
  printf("V2: 0x%02x  V6: 0x%02x  VA: 0x%02x  VE: 0x%02x\n", V[2], V[6], V[10], V[14]);
  printf("V3: 0x%02x  V7: 0x%02x  VB: 0x%02x  VF: 0x%02x\n", V[3], V[7], V[11], V[15]);

  printf("I: 0x%04x", I);
  printf("\n");
  printf("PC: 0x%04x OP: 0x%04x\n", pc, opcode());
  printf("\n");
  printf("\n");
}
}  // namespace emu::chip8
