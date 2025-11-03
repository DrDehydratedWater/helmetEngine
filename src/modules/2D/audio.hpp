#pragma once
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../../engine.hpp"
#include <string>
#include <unordered_map>

/// Caches / plays sound effects and music using SDL_mixer
class AudioModule : public Module {
public:
  AudioModule() : Module("AudioModule") {}

  std::pair<const char*, Mix_Music*> music;
  std::unordered_map<std::string, Mix_Chunk*> soundEffects;
  std::unordered_map<int, bool> channelStatus;

  void audioInit() {
    SDL_Init(SDL_INIT_AUDIO);

    Mix_Init(MIX_INIT_MP3);

    Mix_OpenAudio(0, NULL);
  }

  void playSFX(int channel, const char* const& path, int loops) {
    if (soundEffects.find(path) == soundEffects.end()) {
      Mix_Chunk* chunk = Mix_LoadWAV(path);
      
      soundEffects[path] = chunk;
    }

    Mix_PlayChannel(channel, soundEffects[path], loops);

    channelStatus[channel] = true;
  }

  void playMusic(const char* const& path, int loops) {
    if (music.first != path) {
      music.first = path;
      music.second = Mix_LoadMUS(path);
    }

    Mix_PlayMusic(music.second, loops);
  }

  void startup(Engine* engine) override {
    audioInit();
  }

  bool isChannelPlaying(int channel) {
    if (Mix_Playing(channel) == 0) {
      return false;
    }
    return true;
  }
  
  bool isMusicPlaying() {
    if (Mix_PlayingMusic() == 0) {
      return false;
    }
    return true;
  }
};