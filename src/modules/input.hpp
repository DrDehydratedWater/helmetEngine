#include "../engine.hpp"

class Input {
public:
  std::unordered_map<SDL_Keycode, bool> keyStates;
  
  bool isKeyDown(SDL_Keycode key) {
    return keyStates[key];
  }
};

class InputModule : public Module {
public:
  Input* input;

  InputModule(Input* i) : input(i) {}

  void startup(Engine* engine) override {}

  void main(Engine* engine) override {
    while (SDL_PollEvent(&engine->event)) {
      if (engine->event.type == SDL_EVENT_QUIT) {
        engine->running = false;
        for (Module* module : engine->modules) {
          module->shutdown(engine);
        }
      } else if (engine->event.type == SDL_EVENT_KEY_DOWN) {
        input->keyStates[engine->event.key.key] = true;
      } else if (engine->event.type == SDL_EVENT_KEY_UP) {
        input->keyStates[engine->event.key.key] = false;
      }
    }
  }

  void shutdown(Engine* engine) override {}
};