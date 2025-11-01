#pragma once

#include "scene.hpp"
#include "logger.hpp"
#include <functional>
#include <chrono>
#include <SDL3/SDL_events.h>

class Engine;

class Module {
public:
  virtual void startup(Engine*) {};
  virtual void main(Engine*) {};
  virtual void shutdown(Engine*) {};

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
    Logger::Log("\n-- Initializing engine! --\n");

    process = inputProcess;
    scene = inputScene;
    modules = inputModules;

    Logger::Log("\n-- Initializing modules! --\n");
    for (Module* module : modules) {
      Logger::Log("Initializing module: " + std::string(typeid(*module).name()).substr(2) + "\n");
      module->startup(this);
    }

    Logger::Log("\n-- Initialization finished! --\n");
    engineProcess();
  }

  template <typename T>
  auto findModule() {
    return std::find_if(
      modules.begin(), modules.end(), 
      [](Module* module) {return dynamic_cast<T*>(module) != nullptr;});
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
