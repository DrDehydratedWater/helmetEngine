#pragma once

#include "util/math/vec2.hpp"
#include "util/logger.hpp"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

class GameObject {
public:
  virtual ~GameObject() = default;

  std::string id;

  GameObject* parent = nullptr;

  std::vector<GameObject*> children;


  void addObject(GameObject* inputObject) {
    children.push_back(inputObject);
  }
};

class Scene {
public:
  std::vector<std::unique_ptr<GameObject>> objects;

  auto findObject(std::string id) {
    Logger::Log("Finding object: " + id + "\n");
    return std::find_if(
        objects.begin(), objects.end(),
        [&](const std::unique_ptr<GameObject> &obj) { return obj->id == id; });
  }

  template<typename T>
  auto getObject(std::string id) {
    Logger::Log("Getting object" + id + "\n");
    auto it = std::find_if(
        objects.begin(), objects.end(),
        [&](const std::unique_ptr<GameObject> &obj) { return obj->id == id; });
    return (it != objects.end()) ? dynamic_cast<T*>(it->get()) : nullptr;
  }

  template <typename T>
  void addObject(std::unique_ptr<T> inputObject) {
    Logger::Log("Adding object: " + inputObject->id + "\n");
    static_assert(std::is_base_of_v<GameObject, T>, "T must derive from gameObject");
    objects.push_back(std::move(inputObject));
  }

  template <typename T>
  void addObjects(std::vector<std::unique_ptr<T>>&& inputObjects) {
    Logger::Log("Adding objects: " + inputObjects + "\n");
    static_assert(std::is_base_of_v<GameObject, T>, "T must derive from gameObject");
    for (auto& inputObject : inputObjects) {
      Logger::Log("Adding object: " + inputObject->id + "\n");
      objects.push_back(std::move(inputObject));
    }
  }

  void destroy(std::string id) {
    Logger::Log("Destroying object: " + id + "\n");
    auto it = findObject(id);

    if (it != objects.end()) {
      objects.erase(it);
    }
  }
};
