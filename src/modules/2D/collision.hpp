#pragma once
#include "node.hpp"
#include "../../engine.hpp"
#include "../../scene.hpp"
#include "../../util/math/Vec2.hpp"

class PhysicsObject : public Node {
public:
  Vec2 size;
  Vec2 velocity;
  bool enabled;
};

class CollisionModule : public Module {
public:
  CollisionModule() : Module("CollisionModule") {}


  static bool isColliding(const PhysicsObject& obj1, const PhysicsObject& obj2) {
    if (obj1.position.x + obj1.size.x >= obj2.position.x &&
        obj1.position.x <= obj2.position.x + obj2.size.x &&
        obj1.position.y + obj1.size.y >= obj2.position.y &&
        obj1.position.y <= obj2.position.y + obj2.size.y) { return true; }
    return false;
  }

  void main(Engine* engine) override {
    for (auto& obj : engine->scene->objects) {
      if (auto physicsObject = dynamic_cast<PhysicsObject*>(obj.get())) {
        physicsObject->position += physicsObject->velocity;
        for (auto child : physicsObject->children) {
          if (auto node = dynamic_cast<Node*>(child)) {
            node->position = physicsObject->position + node->localPosition;
          }
        }
      }
    }

    for (int i = 0; i < engine->scene->objects.size(); i++) {
      auto& obj1 = engine->scene->objects.at(i);
      if (auto physicsObject1 = dynamic_cast<PhysicsObject*>(obj1.get())) {
        for (int j = i + 1; j < engine->scene->objects.size(); j++) {
          auto& obj2 = engine->scene->objects.at(j);
          if (auto physicsObject2 = dynamic_cast<PhysicsObject*>(obj2.get())) {
            if (isColliding(*physicsObject1, *physicsObject2)) {
              physicsObject1->position -= physicsObject1->velocity;
              for (auto child : physicsObject1->children) {
                if (auto node = dynamic_cast<Node*>(child)) {
                  node->position = physicsObject1->position + node->localPosition;
                }
              }
            }
          }
        }
      }
    }
  }
};