#pragma once
#include <iostream>

class Logger {
public:
  static void Log(std::string s) {
    std::cout << s;
  }
};