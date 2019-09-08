#include "emu/chip8.hpp"
#include "emu/clock.hpp"
#include "emu/program.hpp"
#include "util/configuration.hpp"
#include "util/debug.hpp"
#include "util/log.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
  auto conf = util::parse_args(argc, argv);
  logging::setup(conf);

  logging::info("I'm working with #{} args", argc);

  auto program = emu::Program::read(conf.rom);

  auto clock = std::make_unique<emu::Clock>(60_Hz);
  emu::chip8::Emulator emulator(nullptr, nullptr, clock, program);

  return emulator(true);
}
