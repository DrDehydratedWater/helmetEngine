#include "engine.hpp"
#include "scene.hpp"
#include "modules/2D/input.hpp"
#include "modules/2D/renderer.hpp"
#include "modules/2D/audio.hpp"
#include "modules/2D/collision.hpp"
#include <iostream>

void process(Engine* engine, double deltaTime) {
  InputModule* inputModule = dynamic_cast<InputModule*>(*engine->findModule<InputModule>());
  
  PhysicsObject* player = dynamic_cast<PhysicsObject*>(engine->scene->findObject("player")->get());
  Sprite* playerSprite = dynamic_cast<Sprite*>(engine->scene->findObject("playerSprite")->get());

  AudioModule* audioModule = dynamic_cast<AudioModule*>(*engine->findModule<AudioModule>());
  
  double speed = 0.05;

  player->velocity = {0, 0};

  if (inputModule->isKeyDown(SDLK_W)) {
    player->velocity.y = -speed;
  }
  if (inputModule->isKeyDown(SDLK_S)) {
    player->velocity.y = speed;
  }
  if (inputModule->isKeyDown(SDLK_D)) {
    player->velocity.x = speed;
  }
  if (inputModule->isKeyDown(SDLK_A)) {
    player->velocity.x = -speed;
  }
  if (inputModule->isKeyDown(SDLK_F)) {
    if (!audioModule->isPlaying("../resources/sample.wav")) {
      audioModule->playAudio("../resources/sample.wav");
    }
    playerSprite->size += {0.005, 0.005};
    player->size += {0.005, 0.005};
  }
  if (inputModule->isKeyDown(SDLK_G)) {
    playerSprite->size -= {0.005, 0.005};
    player->size -= {0.005, 0.005};
  }
}

int main() {
  RendererModule rendererModule;
  rendererModule.rendererInit("Sprite Test", 1000, 1000);

  AudioModule audioModule;
  audioModule.audioInit();
  
  InputModule inputModule;

  CollisionModule collisionModule;

  Scene scene;

  auto player = std::make_unique<PhysicsObject>();
  player->id = "player";
  player->size = {128, 128};
  player->position = {0, 0};

  auto sprite = std::make_unique<Sprite>();
  sprite->id = "playerSprite";
  sprite->position = {0, 0};
  sprite->size = {128, 128};
  sprite->texture = IMG_LoadTexture(rendererModule.SDLRenderer, "../resources/icon.png");

  player->addObject(sprite.get());

  auto staticObj = std::make_unique<PhysicsObject>();
  staticObj->id = "staticObject";
  staticObj->size = {128, 128};
  staticObj->position = {200, 200};
  staticObj->velocity = {0, 0};

  auto staticSprite = std::make_unique<Sprite>();
  staticSprite->id = "staticSprite";
  staticSprite->position = staticObj->position;
  staticSprite->size = {128, 128};
  staticSprite->texture = IMG_LoadTexture(rendererModule.SDLRenderer, "../resources/icon.png");

  staticObj->addObject(staticSprite.get());

  scene.addObject(std::move(sprite));
  scene.addObject(std::move(player));
  scene.addObject(std::move(staticSprite));
  scene.addObject(std::move(staticObj));

  Engine engine;

  engine.engineInit(process, &scene, {&audioModule, &rendererModule, &inputModule, &collisionModule});

  return 0;
}
