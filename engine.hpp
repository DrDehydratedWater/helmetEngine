#pragma once

#include "scene.hpp"
#include <functional>
#include <SDL3/SDL_events.h>

class Module {
public:
  virtual void main(Scene*) = 0;
  virtual ~Module() = default;
};

class Engine {
public:
  std::function<void(Scene*)> process;
  Scene *scene;
  SDL_Event event;

  bool running = true;

  void engineInit(std::function<void(Scene*)> inputProcess, Scene *inputScene, std::vector<Module*> inputModules = {}) {
    process = inputProcess;
    scene = inputScene;
    modules = inputModules;
    engineProcess();
  }

private:
  std::vector<Module*> modules;
  void engineProcess() {
    while (running) {
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_EVENT_QUIT) {
              running = false;
          }
      }

      process(scene);

      for (Module* module : modules) {
        module->main(scene);
      }
    }
  }
};
