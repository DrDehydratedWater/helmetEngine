#pragma once
#include "../../scene.hpp"

class Node : public GameObject {
public:
  Vec2 position = {0, 0};
  Vec2 localPosition = {0, 0};
};