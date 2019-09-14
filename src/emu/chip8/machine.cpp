#include "emu/chip8/machine.hpp"

#include "emu/clock/clock.hpp"
#include "emu/program.hpp"
#include "util/debug.hpp"
#include "util/log.hpp"
#include "util/memory.hpp"
#include "util/ptr.hpp"

#include <algorithm>

namespace emu::chip8 {

namespace op {
}

void Machine::reset(emu::Program const& program, Ptr<Random<uint8_t>> rd)
{
  pc = 0x200;
  I = 0;
  sp = 0;

  redraw = true;

  randomDevice = rd;

  mem::zero(memory);
  // mem::zero(instructions);
  mem::zero(V);
  mem::zero(display);
  mem::zero(stack);
  mem::zero(key);

  std::copy(std::begin(fontset), std::end(fontset), memory.begin());
  std::copy(program.begin(), program.end(), std::next(memory.begin(), 512));
  logging::debug("Program is: \n{}", debug::dump_memory(program.content.data(), program.size()));

  parseInstructions();
}

void Machine::parseInstructions()
{
  for (std::size_t pc = 512; pc < 4096; pc += 2)
  {
    uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
    instructions[(pc - 512) / 2] = op::parse(opcode);
  }
}

bool Machine::cycle(bool sanitize)
{
  try
  {
    logging::trace("pc={:#0x}", pc);
    execute();
  }
  catch (std::exception const& ex)
  {
    logging::error("Emulation failed because of {}. [pc={:#0x}]", ex.what(), pc);
    logging::debug("I={:#0x}", I);
    logging::debug("sp={:#0x}, stack=\n{}", sp, debug::dump_memory(stack));
    logging::debug("V=\n{}", debug::dump_memory(V));
    logging::debug("memory=\n{}", debug::dump_memory(memory));

    if (sanitize)
      return false;
  }

  if (delayTimer > 0)
    --delayTimer;

  if (soundTimer > 0)
    --soundTimer;
  sound = soundTimer != 0;

  return true;
}

void Machine::clearDisplay()
{
  mem::zero(display);
}

void Machine::execute()
{
  auto const& instr = instructions[(pc - 512) / 2];

  instr.op(*this);

  if (!instr.jump)
    pc += 2;
}
}  // namespace emu::chip8
