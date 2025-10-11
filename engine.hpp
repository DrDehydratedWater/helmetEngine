#pragma once

#include "scene.hpp"
#include <functional>
#include <chrono>
#include <SDL3/SDL_events.h>

class Module {
public:
  virtual void main(Scene*) = 0;
  virtual void shutdown(Scene*) = 0;
  virtual void startup(Scene*) = 0;

  virtual ~Module() = default;
};

class Engine {
public:
  std::function<void(Scene*, double)> process;
  Scene *scene;
  SDL_Event event;

  bool running = true;

  void engineInit(std::function<void(Scene*, double)> inputProcess, Scene *inputScene, std::vector<Module*> inputModules = {}) {
    process = inputProcess;
    scene = inputScene;
    modules = inputModules;

    for (Module* module : modules) {
      module->startup(scene);
    }

    engineProcess();
  }

private:
  std::vector<Module*> modules;
  double deltaTime;

  void engineProcess() {
    while (running) {
      auto start = std::chrono::high_resolution_clock::now();

      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_EVENT_QUIT) {
              running = false;
              for (Module* module : modules) {
                module->shutdown(scene);
              }
          }
      }

      process(scene, deltaTime);

      for (Module* module : modules) {
        module->main(scene);
      }

      auto stop = std::chrono::high_resolution_clock::now();

      deltaTime = std::chrono::duration<double, std::milli>(stop - start).count();
    }
  }
};
