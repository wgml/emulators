#include "io/audio.hpp"

#include "util/log.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <algorithm>
#include <cmath>

namespace io {

namespace {

void audio_callback(void* pAudio, std::uint8_t* stream, int length)
{
  Audio* audio = (Audio*)pAudio;
  audio->generate((std::int16_t*)stream, length / 2);
}
}  // namespace

Audio::Audio() : active(false), currentFreq(0_Hz)
{
  SDL_AudioSpec desiredSpec;

  desiredSpec.freq = freq.hertz;
  desiredSpec.format = AUDIO_S16SYS;
  desiredSpec.channels = 1;
  desiredSpec.samples = 2048;
  desiredSpec.callback = audio_callback;
  desiredSpec.userdata = this;

  SDL_AudioSpec obtainedSpec;

  // you might want to look for errors here
  SDL_OpenAudio(&desiredSpec, &obtainedSpec);

  // start play audio
  SDL_PauseAudio(0);
}

Audio::~Audio()
{
  SDL_CloseAudio();
}

void Audio::beep(emu::Frequency f)
{
  logging::info("Beep for {}Hz registered", f.hertz);
  SDL_LockAudio();
  {
    currentFreq = f;
    active = true;
  }
  SDL_UnlockAudio();
}

void Audio::endBeep()
{
  SDL_LockAudio();
  {
    currentFreq = 0_Hz;
    active = false;
  }
  SDL_UnlockAudio();
  logging::info("Beep done");
}

void Audio::generate(std::int16_t* stream, int length)
{
  if (!active)
  {
    std::fill(stream, stream + length, 0);
    return;
  }

  for (int i = 0; i < length; ++i)
  {
    double time = 1. * i / currentFreq.hertz;
    stream[i] = amp * std::sin(2. * M_PI * freq.hertz * time);
  }
}
}  // namespace io
