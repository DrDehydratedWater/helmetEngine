#pragma once
#include <iostream>

class Logger {
public:
  static bool enabled;

  static void Log(std::string s) {
    if (enabled) std::cout << s;
  }
};