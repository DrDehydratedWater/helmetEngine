#pragma once

#include "util/math/vec2.hpp"
#include "util/logger.hpp"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

/// @brief An object with hierarchy and a label
class GameObject {
public:
  virtual ~GameObject() = default;

  std::string label;

  GameObject* parent = nullptr;

  std::vector<GameObject*> children;


  /// @brief Adds the object as a child
  /// @param inputObject The child object
  void addObject(GameObject* inputObject) {
    children.push_back(inputObject);
  }
};

/// @brief Stores objects
class Scene {
public:
  std::vector<std::unique_ptr<GameObject>> objects;

  /// @brief Finds an object
  /// @param label A string label of the object
  /// @return An iterator
  auto findObject(std::string label) {
    Logger::Log("Finding object: " + label + "\n");
    return std::find_if(
        objects.begin(), objects.end(),
        [&](const std::unique_ptr<GameObject> &obj) { return obj->label == label; });
  }

  /// @brief Returns a pointer instead of an iterator 
  /// @tparam T The class of the object to be found
  /// @param label A string label of the object
  /// @return A pointer
  template<typename T>
  auto getObject(std::string label) {
    Logger::Log("Getting object" + label + "\n");
    auto it = std::find_if(
        objects.begin(), objects.end(),
        [&](const std::unique_ptr<GameObject> &obj) { return obj->label == label; });
    return (it != objects.end()) ? dynamic_cast<T*>(it->get()) : nullptr;
  }

  /// @brief Adds an object to the scene
  /// @tparam T The class of the object
  /// @param inputObject The object to add
  template <typename T>
  void addObject(std::unique_ptr<T> inputObject) {
    Logger::Log("Adding object: " + inputObject->label + "\n");
    static_assert(std::is_base_of_v<GameObject, T>, "T must derive from gameObject");
    objects.push_back(std::move(inputObject));
  }

  /// @brief Destroys an object in the scene
  /// @param label The label of the object to be removed
  void destroy(std::string label) {
    Logger::Log("Destroying object: " + label + "\n");
    auto it = findObject(label);

    if (it != objects.end()) {
      objects.erase(it);
    }
  }
};
