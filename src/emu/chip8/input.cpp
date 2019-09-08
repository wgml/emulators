#include "emu/chip8/input.hpp"

namespace emu::chip8 {

void Input::update(Machine& machine)
{
  constexpr io::Keys mapping[16] = {io::Keys::Num1,
                                    io::Keys::Num2,
                                    io::Keys::Num3,
                                    io::Keys::Num4,
                                    io::Keys::Q,
                                    io::Keys::W,
                                    io::Keys::E,
                                    io::Keys::R,
                                    io::Keys::A,
                                    io::Keys::S,
                                    io::Keys::D,
                                    io::Keys::F,
                                    io::Keys::Z,
                                    io::Keys::X,
                                    io::Keys::C,
                                    io::Keys::V};

  for (std::size_t k = 0; k < 16; ++k)
    machine.key[k] = keys->isPressed(mapping[k]);
}
}  // namespace emu::chip8
