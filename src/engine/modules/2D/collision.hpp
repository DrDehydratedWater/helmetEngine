#pragma once
#include "../../engine.hpp"
#include "../../scene.hpp"
#include "../../util/math/vec2.hpp"
#include "shapes.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>


class PhysicsObject : public Node {
public:
  std::unique_ptr<Rect> rect;
  Vec2 velocity = {0, 0};
  bool enabled;
  bool isStatic = false; // if true, this object should not be moved by physics/collision resolution
};

class CollisionModule : public Module {
public:
  int maxDepth;
  int maxObjects;

  struct Cell {
    std::vector<Cell> cells;
    std::unique_ptr<Rect> rect;

    std::vector<PhysicsObject*> objects;

    void addCell(std::unique_ptr<Rect> inputRect, const std::vector<PhysicsObject*>& inputObjects) {
      Cell cell;

      cell.rect = std::move(inputRect);
      cell.objects = inputObjects;

      cells.push_back(std::move(cell));
    }
  };


  CollisionModule() : Module("CollisionModule") {}

  void main(Engine* engine) override {
    std::vector<PhysicsObject*> allPhysicsObjects;
    for (auto& obj : engine->scene->objects) {
      if (auto physicsObject = dynamic_cast<PhysicsObject*>(obj.get())) {
        // Only update position from velocity for non-static objects
        if (!physicsObject->isStatic) {
          physicsObject->move(physicsObject->position + physicsObject->velocity * engine->deltaTime);
        } else {
          // ensure rect still matches object's fixed position
          if (physicsObject->rect) physicsObject->rect->position = physicsObject->position;
        }
        if (physicsObject->rect) physicsObject->rect->position = physicsObject->position;
        allPhysicsObjects.push_back(physicsObject);
      }
    }

    Cell root;
    root.rect = std::make_unique<Rect>();
    root.rect->position = {0, 0};
    root.rect->size = {1920, 1080};
    root.objects = allPhysicsObjects;

    subdivideCell(root, 0, engine);

    checkCollisionsInCell(root, engine);
  }

private:
  static bool isColliding(const Rect& a, const Rect& b) {
    if (a.position.x + a.size.x >= b.position.x &&
        a.position.x <= b.position.x + b.size.x &&
        a.position.y + a.size.y >= b.position.y &&
        a.position.y <= b.position.y + b.size.y) { return true; }
    return false;
  }

  void resolveOverlap(PhysicsObject* pa, PhysicsObject* pb) {
    if (!pa || !pb || !pa->rect || !pb->rect) return;
    Rect &a = *pa->rect;
    Rect &b = *pb->rect;

    // compute penetration on each side
    double overlapXL = (a.position.x + a.size.x) - b.position.x;
    double overlapXR = (b.position.x + b.size.x) - a.position.x;
    double overlapYT = (a.position.y + a.size.y) - b.position.y;
    double overlapYB = (b.position.y + b.size.y) - a.position.y;

    // clamp to non-negative
    overlapXL = std::max(0.0, overlapXL);
    overlapXR = std::max(0.0, overlapXR);
    overlapYT = std::max(0.0, overlapYT);
    overlapYB = std::max(0.0, overlapYB);

    double resolveXAmount = std::min(overlapXL, overlapXR);
    double resolveYAmount = std::min(overlapYT, overlapYB);

    // move the PhysicsObject positions (so changes persist across frames)
    bool aStatic = pa->isStatic;
    bool bStatic = pb->isStatic;
    if (aStatic && bStatic) {
      // both static -> nothing to do
    } else if (resolveXAmount < resolveYAmount) {
      // resolve on X axis
      if (overlapXL < overlapXR) {
        // a penetrated b from the left
        if (aStatic) {
          // move b fully
          pb->position.x += resolveXAmount;
        } else if (bStatic) {
          // move a fully
          pa->position.x -= resolveXAmount;
        } else {
          pa->position.x -= resolveXAmount * 0.5;
          pb->position.x += resolveXAmount * 0.5;
        }
      } else {
        // a penetrated b from the right
        if (aStatic) {
          pb->position.x -= resolveXAmount;
        } else if (bStatic) {
          pa->position.x += resolveXAmount;
        } else {
          pa->position.x += resolveXAmount * 0.5;
          pb->position.x -= resolveXAmount * 0.5;
        }
      }
    } else {
      // resolve on Y axis
      if (overlapYT < overlapYB) {
        // a penetrated b from the top
        if (aStatic) {
          pb->position.y += resolveYAmount;
        } else if (bStatic) {
          pa->position.y -= resolveYAmount;
        } else {
          pa->position.y -= resolveYAmount * 0.5;
          pb->position.y += resolveYAmount * 0.5;
        }
      } else {
        // a penetrated b from the bottom
        if (aStatic) {
          pb->position.y -= resolveYAmount;
        } else if (bStatic) {
          pa->position.y += resolveYAmount;
        } else {
          pa->position.y += resolveYAmount * 0.5;
          pb->position.y -= resolveYAmount * 0.5;
        }
      }
    }

    // keep rect positions in sync with their owners
    pa->rect->position = pa->position;
    pb->rect->position = pb->position;
  }


  void subdivideCell(Cell& cell, int depth, Engine* engine) {
    if (depth >= maxDepth || cell.objects.size() <= maxObjects) {
      return;
    }

    Vec2 cellHalfSize = cell.rect->size * 0.5;
    Vec2 cellPos = cell.rect->position;

    Vec2 quadPositions[4] = {
      cellPos, // top-left
      {cellPos.x + cellHalfSize.x, cellPos.y}, // top-right
      {cellPos.x, cellPos.y + cellHalfSize.y}, // bottom-left
      {cellPos.x + cellHalfSize.x, cellPos.y + cellHalfSize.y} // bottom-right
    };

    for (int i = 0; i < 4; i++) {
      std::vector<PhysicsObject*> quadObjects;
      auto quadRect = std::make_unique<Rect>();
      quadRect->position = quadPositions[i];
      quadRect->size = cellHalfSize;

      for (auto obj : cell.objects) {
        if (!obj->rect) continue;
        if (isColliding(*quadRect, *obj->rect)) {
          quadObjects.push_back(obj);
        }
      }

      cell.addCell(std::move(quadRect), quadObjects);
      subdivideCell(cell.cells[i], depth + 1, engine);
    }
  }

  void checkCollisionsInCell(const Cell& cell, Engine* engine) {
    for (int i = 0; i < cell.objects.size(); i++) {
      for (int j = i + 1; j < cell.objects.size(); j++) {
        PhysicsObject* pa = cell.objects[i];
        PhysicsObject* pb = cell.objects[j];
        if (!pa || !pb || !pa->rect || !pb->rect) continue;
        if (isColliding(*pa->rect, *pb->rect)) {
          resolveOverlap(pa, pb);
        }
      }
    }

    for (const auto& subcell : cell.cells) {
      checkCollisionsInCell(subcell, engine);
    }
  }
};