#pragma once
#include "../../util/math/vec2.hpp"
#include "../../scene.hpp"

class Node : public GameObject {
public:
  Vec2 localPosition = {0, 0};
  Vec2 position = localPosition;

  void move(Vec2 p) {
    position = p + localPosition;
    for (auto child : children) {
      if (Node* node = dynamic_cast<Node*>(child)) {
        node->move(p + node->localPosition);
      }
    }
  }
};

class Box : public Node {
public:
  Vec2 size;
};