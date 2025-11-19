#pragma once
#include <iostream>

/// @brief Generic logger
class Logger {
public:
  static bool enabled;

  /// If the logger is enabled prints the string to iostream
  static void Log(std::string s) {
    if (enabled) std::cout << s;
  }
};