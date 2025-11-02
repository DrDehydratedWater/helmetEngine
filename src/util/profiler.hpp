#pragma once
#include <chrono>
#include <unordered_map>
#include <vector>
#include <string>
#include "logger.hpp"

class Profiler {
  struct Data {
    std::vector<long long> times;
    std::chrono::high_resolution_clock::time_point start;
  };

  std::unordered_map<std::string, Data> data;

public:
  void start(const std::string& label) {
    data[label].start = std::chrono::high_resolution_clock::now();
  }

  void stop(const std::string& label) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - data[label].start);
    data[label].times.push_back(duration.count());
  }

  void printTimes(const std::string& label) {
    auto it = data.find(label);
    if (it == data.end() || it->second.times.empty()) return;
    for (auto t : it->second.times) Logger::Log(label + " took " + std::to_string(t) + " microseconds\n");
  }

  double getAverage(const std::string& label) const {
    auto it = data.find(label);
    if (it == data.end() || it->second.times.empty()) return 0.0;

    long long total = 0;
    for (auto t : it->second.times) total += t;
    return static_cast<double>(total) / it->second.times.size();
  }

  void printAverage(const std::string& label) {
    Logger::Log(label + " on average took " + std::to_string(getAverage(label)) + " microseconds\n");
  }
};
