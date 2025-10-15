#include "../engine.hpp"

class InputModule : public Module {
public:
  std::unordered_map<SDL_Keycode, bool> keyStates;
  

  bool isKeyDown(SDL_Keycode key) {
    return keyStates[key];
  }


  void main(Engine* engine) override {
    while (SDL_PollEvent(&engine->event)) {
      if (engine->event.type == SDL_EVENT_QUIT) {
        engine->running = false;
        for (Module* module : engine->modules) {
          module->shutdown(engine);
        }
      } else if (engine->event.type == SDL_EVENT_KEY_DOWN) {
        keyStates[engine->event.key.key] = true;
      } else if (engine->event.type == SDL_EVENT_KEY_UP) {
        keyStates[engine->event.key.key] = false;
      }
    }
  }
};