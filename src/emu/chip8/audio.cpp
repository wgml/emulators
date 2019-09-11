#include "emu/chip8/audio.hpp"

#include "emu/clock/freq.hpp"
#include "util/log.hpp"

namespace emu::chip8 {

Audio::Audio(Ptr<io::Audio> audio) : beeping(false), audio(audio)
{}

void Audio::update(Machine const& machine)
{
  logging::trace("audio: beeping={}, machine.sound={}", beeping, machine.sound);
  if (beeping == machine.sound)
    return;

  if (machine.sound)
  {
    audio->beep(2000_Hz);
    beeping = true;
  }
  else
  {
    audio->endBeep();
    beeping = false;
  }
}

}  // namespace emu::chip8
