#include "emu/chip8.hpp"
#include "emu/clock.hpp"
#include "emu/program.hpp"
#include "io/display.hpp"
#include "util/configuration.hpp"
#include "util/debug.hpp"
#include "util/log.hpp"

#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
  auto conf = util::parse_args(argc, argv);
  logging::setup(conf);

  logging::info("I'm working with #{} args", argc);

  auto display = std::make_unique<io::Display>(64, 32);
  auto program = emu::Program::read(conf.rom);

  auto emuDisplay = std::make_unique<emu::chip8::Display>(display->screen());
  auto emuInput = std::make_unique<emu::chip8::Input>(display->input());

  auto clock = std::make_unique<emu::Clock>(60_Hz, "emulator");

  emu::chip8::Emulator emulator(emuDisplay, emuInput, clock, program);
  std::thread emuThread{[&emulator] { emulator(true); }};

  auto guiClock = std::make_unique<emu::Clock>(144_Hz, "interface");
  while (!display->closed())
  {
    guiClock->tick();
    display->frame();
  }

  emulator.stop();
  emuThread.join();
  return 0;
}
