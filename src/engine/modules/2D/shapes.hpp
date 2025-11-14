#pragma once
#include "../../util/math/vec2.hpp"
#include "../../scene.hpp"

class Node : public GameObject {
public:
  Vec2 localPosition = {0, 0};
  Vec2 position = localPosition;

  Node() = default;

  Node(const Vec2& position_)
    : localPosition(0, 0), position(position_) {}

  void move(Vec2 p) {
    position = p + localPosition;
    for (auto child : children) {
      if (Node* node = dynamic_cast<Node*>(child)) {
        node->move(p + node->localPosition);
      }
    }
  }
};

class Rect : public Node {
public:
  Vec2 size;

  Rect() = default;
  
  Rect(const Vec2& size_, const Vec2& position_ = {0, 0})
    : Node(position_), size(size_) {}
};