#pragma once

#include <algorithm>
#include <dehydrated/math/vector.h>
#include <memory>
#include <string>
#include <vector>

class GameObject {
public:
  virtual ~GameObject() = default;

  std::string id;
  Vec2 position;
};

class Scene {
public:
  std::vector<std::unique_ptr<GameObject>> objects;

  auto findObject(std::string id) {
    return std::find_if(
        objects.begin(), objects.end(),
        [&](const std::unique_ptr<GameObject> &obj) { return obj->id == id; });
  }

  template <typename T> void addObject(std::unique_ptr<T> inputObject) {
    static_assert(std::is_base_of_v<GameObject, T>,
                  "T must derive from gameObject");
    objects.push_back(std::unique_ptr<GameObject>(inputObject.release()));
  }

  template <typename T>
  void addObject(std::vector<std::unique_ptr<T>> inputObjects) {
    for (auto inputObject : inputObjects) {
      static_assert(std::is_base_of_v<GameObject, T>,
                    "T must derive from gameObject");
      objects.push_back(std::unique_ptr<GameObject>(inputObject.release()));
    }
  }

  void destroy(std::string id) {
    auto it = findObject(id);

    if (it != objects.end()) {
      objects.erase(it);
    }
  }
};
