#pragma once

#include "scene.hpp"
#include "util/logger.hpp"
#include "util/profiler.hpp"
#include <functional>
#include <chrono>
#include <SDL3/SDL_events.h>
#include <iostream>

class Engine;

class Module {
public:
  const std::string id;
  Module(const std::string& moduleId) : id(moduleId) {}

  virtual void startup(Engine*) {};
  virtual void main(Engine*) {};
  virtual void shutdown(Engine*) {};

  virtual ~Module() = default;
};

class Engine {
public:
  std::vector<std::unique_ptr<Module>> modules;
  std::function<void(Engine*, double)> process;
  Scene *scene;
  SDL_Event event;
  Profiler profiler;

  double deltaTime;

  bool running = true;

  /// Takes a function to be ran every frame, a scene and a vector of unique pointers to module objects.
  void engineInit(std::function<void(Engine*, double)> inputProcess, Scene *inputScene, std::vector<std::unique_ptr<Module>> inputModules = {}) {
    profiler.start("\nEngine initialization");
    Logger::Log("\n-- Initializing engine! --\n");

    process = inputProcess;
    scene = inputScene;
    modules = std::move(inputModules);

    Logger::Log("\n-- Initializing modules! --\n");
    for (std::unique_ptr<Module>& module : modules) {
      Logger::Log("Initializing module: " + std::string(typeid(*module).name()).substr(2) + "\n");
      module->startup(this);
    }

    Logger::Log("\n-- Initialization finished! --\n");
    profiler.stop("\nEngine initialization");
    profiler.printTimes("\nEngine initialization");
    engineProcess();
  }

  auto findModule(std::string id) {
    Logger::Log("Finding module: " + id + "\n");
    return std::find_if(
        modules.begin(), modules.end(),
        [&](const std::unique_ptr<Module> &module) { return module->id == id; });
  }

  template<typename T>
  auto getModule(std::string id) {
    Logger::Log("Getting module" + id + "\n");
    auto it = std::find_if(
        modules.begin(), modules.end(),
        [&](const std::unique_ptr<Module> &module) { return module->id == id; });
    return (it != modules.end()) ? dynamic_cast<T*>(it->get()) : nullptr;
  }

  void shutdown() {
    profiler.start("\nShutting down engine");
    Logger::Log("\n-- Shutting down engine --\n");
    running = false;
    Logger::Log("\n-- Shutting down modules --\n");
    for (auto it = modules.rbegin(); it != modules.rend(); ++it) {
      std::unique_ptr<Module>& module = (*it);

      Logger::Log("Shutting down module: " + module->id + "\n");
      module->shutdown(this);

      profiler.printAverage(module->id);
    }
    profiler.stop("\nShutting down engine");
    profiler.printTimes("\nShutting down engine");
  }

private:
  void engineProcess() {
    while (running) {
      auto start = std::chrono::high_resolution_clock::now();


      process(this, deltaTime);

      for (std::unique_ptr<Module>& module : modules) {
        profiler.start(module->id);
        module->main(this);
        profiler.stop(module->id);
      }


      auto stop = std::chrono::high_resolution_clock::now();

      deltaTime = std::chrono::duration<double, std::milli>(stop - start).count();
    }
  }
};
