#pragma once

#include "scene.hpp"
#include "util/logger.hpp"
#include "util/profiler.hpp"
#include <functional>
#include <chrono>
#include <iostream>

class Engine;

class Module {
public:
  const std::string label;
  Module(const std::string& moduleLabel) : label(moduleLabel) {}

  virtual void startup(Engine*) {};
  virtual void main() {};
  virtual void shutdown() {};

  virtual ~Module() = default;
};

class Engine {
public:
  std::vector<std::unique_ptr<Module>> modules;
  std::function<void(Engine*, double)> process;
  Scene *scene;

  double deltaTime;

  bool running = true;

  /// Takes a function to be ran every frame, a scene and a vector of unique pointers to module objects.
  void engineInit(std::function<void(Engine*, double)> inputProcess, Scene *inputScene, std::vector<std::unique_ptr<Module>> inputModules = {}) {
    Profiler::start("\nEngine initialization");
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
    Profiler::stop("\nEngine initialization");
    Profiler::printTimes("\nEngine initialization");
    engineProcess();
  }

  auto findModule(std::string label) {
    Logger::Log("Finding module: " + label + "\n");
    return std::find_if(
        modules.begin(), modules.end(),
        [&](const std::unique_ptr<Module> &module) { return module->label == label; });
  }

  template<typename T>
  auto getModule(std::string label) {
    Logger::Log("Getting module" + label + "\n");
    auto it = findModule(label);
    return (it != modules.end()) ? dynamic_cast<T*>(it->get()) : nullptr;
  }

  void shutdown() {
    Profiler::start("\nShutting down engine");
    Logger::Log("\n-- Shutting down engine --\n");
    running = false;
    Logger::Log("\n-- Shutting down modules --\n");
    for (auto it = modules.rbegin(); it != modules.rend(); ++it) {
      std::unique_ptr<Module>& module = (*it);

      Logger::Log("Shutting down module: " + module->label + "\n");
      module->shutdown();

      Profiler::printAverage(module->label);
    }
    Profiler::stop("\nShutting down engine");
    Profiler::printTimes("\nShutting down engine");
  }

private:
  void engineProcess() {
    while (running) {
      auto start = std::chrono::high_resolution_clock::now();


      for (std::unique_ptr<Module>& module : modules) {
        Profiler::start(module->label);
        module->main();
        Profiler::stop(module->label);
      }

      process(this, deltaTime);


      auto stop = std::chrono::high_resolution_clock::now();

      deltaTime = std::chrono::duration<double>(stop - start).count();
    }
  }
};
