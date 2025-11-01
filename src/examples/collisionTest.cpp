#include "../engine.hpp"
#include "../scene.hpp"
#include "../modules/2D/input.hpp"
#include "../modules/2D/renderer.hpp"
#include "../modules/2D/audio.hpp"
#include "../modules/2D/collision.hpp"
#include "../util/logger.hpp"
#include "../util/uniqueHelper.hpp"
#include <iostream>

bool Logger::enabled = true;

void process(Engine* engine, double deltaTime) {
  static auto* inputModule = engine->getModule<InputModule>("InputModule");
  static auto* audioModule = engine->getModule<AudioModule>("AudioModule");

  static auto* player = engine->scene->getObject<PhysicsObject>("player");
  static auto* playerSprite = engine->scene->getObject<Sprite>("playerSprite");
  
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
  if (inputModule->isKeyDown(SDLK_Q)) {
    engine->shutdown();
  }
}

int main() {
  auto rendererModule = std::make_unique<RendererModule>();
  auto audioModule = std::make_unique<AudioModule>();

  rendererModule->rendererInit("Sprite Test", 1000, 1000);
  audioModule->audioInit();
  

  Scene scene;

  auto player = std::make_unique<PhysicsObject>();
  player->id = "player";
  player->size = {128, 128};
  player->position = {0, 0};

  auto sprite = std::make_unique<Sprite>();
  sprite->id = "playerSprite";
  sprite->position = {0, 0};
  sprite->localPosition = {0, 0};
  sprite->size = {128, 128};
  sprite->texture = IMG_LoadTexture(rendererModule->SDLRenderer, "../resources/icon.png");

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
  staticSprite->texture = IMG_LoadTexture(rendererModule->SDLRenderer, "../resources/icon.png");

  staticObj->addObject(staticSprite.get());

  scene.addObject(std::move(sprite));
  scene.addObject(std::move(player));
  scene.addObject(std::move(staticSprite));
  scene.addObject(std::move(staticObj));

  Engine engine;

  engine.engineInit(process, &scene,
  make_unique_vector<Module>(
    std::move(rendererModule),
    std::move(audioModule),
    std::make_unique<InputModule>(),
    std::make_unique<CollisionModule>()
    )
  );

  return 0;
}