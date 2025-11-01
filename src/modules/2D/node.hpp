#pragma once
#include "../../scene.hpp"

class Node : public GameObject {
public:
  Vec2 localPosition = {0, 0};
  Vec2 position = localPosition;

  void move(Vec2 i) {
    position = i + localPosition;
    for (auto child : children) {
      if (Node* node = dynamic_cast<Node*>(child)) {
        node->position = i + node->localPosition;
      }
    }
  }
};