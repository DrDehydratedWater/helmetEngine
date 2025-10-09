#pragma once

#include "scene.h"
#include <functional>

class engine {
public:
  std::function<void()> process;
  bool running = true;

  void engineInit(std::function<void()> inputProcess) {
    process = inputProcess;
    engineProcess();
  }

private:
  void engineProcess() {
    while (running) {
      process();
    }
  }
};
