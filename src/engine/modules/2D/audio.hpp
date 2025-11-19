#pragma once
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../../engine.hpp"
#include <string>
#include <unordered_map>

/// @brief Caches / plays sound effects and music using SDL_mixer
class AudioModule : public Module {
public:
  AudioModule() : Module("AudioModule") {}

  /// @brief Plays a sound effect
  /// @param channel The integer label of the channel to play the sound effect on 
  /// @param path The path of the sound file
  /// @param loops The number of loops
  void playSFX(int channel, const char* const& path, int loops) {
    if (soundEffects.find(path) == soundEffects.end()) {
      Mix_Chunk* chunk = Mix_LoadWAV(path);
      
      soundEffects[path] = chunk;
    }

    Mix_PlayChannel(channel, soundEffects[path], loops);

    channelStatus[channel] = true;
  }

  /// @brief Plays music, only one piece of music can be playing at a time
  /// @param path The path of the sound file
  /// @param loops The number of loops
  void playMusic(const char* const& path, int loops) {
    if (music.first != path) {
      music.first = path;
      music.second = Mix_LoadMUS(path);
    }

    Mix_PlayMusic(music.second, loops);
  }

  /// @brief Checks if a channel us currently playing sound
  /// @param channel The integer label of the channel
  /// @return Returns true if the channel is playing if not it returns false
  bool isChannelPlaying(int channel) {
    if (Mix_Playing(channel) == 0) {
      return false;
    }
    return true;
  }
  
  /// @brief Checks if music is playing
  /// @return Returns true if music is playing if not it returns false
  bool isMusicPlaying() {
    if (Mix_PlayingMusic() == 0) {
      return false;
    }
    return true;
  }

private:
  std::pair<const char*, Mix_Music*> music;
  std::unordered_map<std::string, Mix_Chunk*> soundEffects;
  std::unordered_map<int, bool> channelStatus;

  void startup(Engine* s) override {
    SDL_Init(SDL_INIT_AUDIO);

    Mix_Init(MIX_INIT_MP3);

    Mix_OpenAudio(0, NULL);
  }
};