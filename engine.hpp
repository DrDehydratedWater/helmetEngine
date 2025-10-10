#pragma once

#include "scene.hpp"
#include <functional>

class Module {
public:
  virtual void main(Scene*) = 0;
  virtual ~Module() = default;
};

class Engine {
public:
  std::function<void()> process;
  Scene *scene;

  bool running = true;

  void engineInit(std::function<void()> inputProcess, Scene *inputScene, std::vector<Module*> = {}) {
    process = inputProcess;
    scene = inputScene;
    engineProcess();
  }

private:
  std::vector<Module*> modules;
  void engineProcess() {
    while (running) {
      process();

      for (Module* module : modules) {
        module->main(scene);
      }
    }
  }
};
