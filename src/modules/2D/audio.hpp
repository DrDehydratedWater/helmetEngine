#pragma once
#include <SDL3/SDL.h>
#include "../../engine.hpp"
#include <string>

class AudioModule : public Module {
public:
  SDL_AudioStream* stream = nullptr;
  SDL_AudioSpec spec;
  Uint8* buffer = nullptr;
  Uint32 length = 0;
  std::string currentFile;

  void audioInit() {
    SDL_Init(SDL_INIT_AUDIO);
  }

  void playAudio(const char* path) {
    SDL_zero(spec);
    spec.freq = 44100;
    spec.format = SDL_AUDIO_F32LE;
    spec.channels = 2;

    currentFile = path;

    if (!SDL_LoadWAV(path, &spec, &buffer, &length)) {
        SDL_Log("Failed to load WAV: %s", SDL_GetError());
        return;
    }

    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
    if (!stream) {
        SDL_Log("Failed to open audio stream: %s", SDL_GetError());
        return;
    }

    SDL_ResumeAudioStreamDevice(stream);
  }


  void main(Engine* engine) override {
    if (SDL_GetAudioStreamQueued(stream) < (int)length) {
      SDL_PutAudioStreamData(stream, buffer, length);
      buffer = nullptr;
    }
  }

  void shutdown(Engine* engine) override {
    SDL_free(buffer);
  }


  bool isPlaying(const char* path) {
    return (currentFile == path) && (SDL_GetAudioStreamQueued(stream) > 0);
  }
};