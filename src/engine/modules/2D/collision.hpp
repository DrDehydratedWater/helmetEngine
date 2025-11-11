#pragma once
#include "../../engine.hpp"
#include "../../scene.hpp"
#include "../../util/math/vec2.hpp"
#include "shapes.hpp"


class PhysicsObject : public Node {
public:
  std::unique_ptr<Box> box;
  Vec2 velocity = {0, 0};
  bool enabled;
};

class CollisionModule : public Module {
public:
  CollisionModule() : Module("CollisionModule") {}


  void isTouching() {
    
  }

  // TODO: Quadtree
  void main(Engine* engine) override {
    for (auto& obj : engine->scene->objects) {
      if (auto physicsObject = dynamic_cast<PhysicsObject*>(obj.get())) {
        physicsObject->move(physicsObject->position + physicsObject->velocity * engine->deltaTime);
        if (physicsObject->box) physicsObject->box->position = physicsObject->position;
      }
    }

    for (int i = 0; i < engine->scene->objects.size(); i++) {
      auto& obj1 = engine->scene->objects.at(i);
      if (auto physicsObject1 = dynamic_cast<PhysicsObject*>(obj1.get())) {
        for (int j = i + 1; j < engine->scene->objects.size(); j++) {
          auto& obj2 = engine->scene->objects.at(j);
          if (auto physicsObject2 = dynamic_cast<PhysicsObject*>(obj2.get())) {
            if (isColliding(*physicsObject1->box, *physicsObject2->box)) {
              std::cout << physicsObject1->id << " is touching " << physicsObject2->id << "\n";
              physicsObject1->move(physicsObject1->position - physicsObject1->velocity * engine->deltaTime);
              if (physicsObject1->box) physicsObject1->box->position = physicsObject1->position;
            }
          }
        }
      }
    }
  }

private:
  static bool isColliding(const Box& obj1, const Box& obj2) {
    if (obj1.position.x + obj1.size.x >= obj2.position.x &&
        obj1.position.x <= obj2.position.x + obj2.size.x &&
        obj1.position.y + obj1.size.y >= obj2.position.y &&
        obj1.position.y <= obj2.position.y + obj2.size.y) { return true; }
    return false;
  }
};