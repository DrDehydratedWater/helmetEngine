#pragma once
#include "../../engine.hpp"

class InputModule : public Module {
public:
  InputModule() : Module("InputModule") {}


  std::unordered_map<SDL_Keycode, bool> keyStates;
  

  bool isKeyDown(SDL_Keycode key) {
    return keyStates.find(key) != keyStates.end() ? keyStates[key] : false;
  }


  void main(Engine* engine) override {
    while (SDL_PollEvent(&engine->event)) {
      if (engine->event.type == SDL_EVENT_QUIT) {
        engine->shutdown();
      } else if (engine->event.type == SDL_EVENT_KEY_DOWN) {
        keyStates[engine->event.key.key] = true;
      } else if (engine->event.type == SDL_EVENT_KEY_UP) {
        keyStates[engine->event.key.key] = false;
      }
    }
  }
};