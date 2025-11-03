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

  inline static std::unordered_map<std::string, Data> data;

public:
  static bool enabled;

  static void start(const std::string& label) {
    if (!enabled) return;
    data[label].start = std::chrono::high_resolution_clock::now();
  }

  static void stop(const std::string& label) {
    if (!enabled) return;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - data[label].start);
    data[label].times.push_back(duration.count());
  }

  static void printTimes(const std::string& label) {
    if (!enabled) return;
    auto it = data.find(label);
    if (it == data.end() || it->second.times.empty()) return;
    for (auto t : it->second.times) Logger::Log(label + " took " + std::to_string(t) + " microseconds\n");
  }

  static double getAverage(const std::string& label) {
    if (!enabled) return 0.0;
    auto it = data.find(label);
    if (it == data.end() || it->second.times.empty()) return 0.0;

    long long total = 0;
    for (auto t : it->second.times) total += t;
    return static_cast<double>(total) / it->second.times.size();
  }

  static void printAverage(const std::string& label) {
    if (!enabled) return;
    Logger::Log(label + " on average took " + std::to_string(getAverage(label)) + " microseconds\n");
  }
};
