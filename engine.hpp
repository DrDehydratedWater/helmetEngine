#pragma once

#include "scene.hpp"
#include <functional>
#include <chrono>
#include <SDL3/SDL_events.h>

class Engine;

class Module {
public:
  virtual void startup(Engine*) = 0;
  virtual void main(Engine*) = 0;
  virtual void shutdown(Engine*) = 0;

  virtual ~Module() = default;
};

class Engine {
public:
  std::vector<Module*> modules;
  std::function<void(Engine*, double)> process;
  Scene *scene;
  SDL_Event event;

  bool running = true;

  void engineInit(std::function<void(Engine*, double)> inputProcess, Scene *inputScene, std::vector<Module*> inputModules = {}) {
    process = inputProcess;
    scene = inputScene;
    modules = inputModules;

    for (Module* module : modules) {
      module->startup(this);
    }

    engineProcess();
  }

private:
  double deltaTime;

  void engineProcess() {
    while (running) {
      auto start = std::chrono::high_resolution_clock::now();


      process(this, deltaTime);

      for (Module* module : modules) {
        module->main(this);
      }


      auto stop = std::chrono::high_resolution_clock::now();

      deltaTime = std::chrono::duration<double, std::milli>(stop - start).count();
    }
  }
};
