#pragma once
#include "../../engine.hpp"
#include <SDL3/SDL_events.h>

/// @brief Captures input and saves the state of each key 
class InputModule : public Module {
private:
  SDL_Event event;
  Engine* engine;

  std::unordered_map<SDL_Keycode, bool> keyStates;
  
  void startup(Engine* temp) {
    engine = temp;
  }

  void main() override {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        engine->shutdown();
      } else if (event.type == SDL_EVENT_KEY_DOWN) {
        keyStates[event.key.key] = true;
      } else if (event.type == SDL_EVENT_KEY_UP) {
        keyStates[event.key.key] = false;
      }
    }
  }

public:
  InputModule() : Module("InputModule") {}


  /// @brief Checks if a key is currently down
  /// @param key SDL_Keycode ( SDLK_*KEY* )
  /// @return Returns true if a key is currently down, if the key cannot be found then it returns false
  bool isKeyDown(SDL_Keycode key) {
    return keyStates.find(key) != keyStates.end() ? keyStates[key] : false;
  }
};