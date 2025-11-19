#include <helmetEngine/engine.hpp>
#include <helmetEngine/scene.hpp>
#include <helmetEngine/modules/2D/input.hpp>
#include <helmetEngine/modules/2D/renderer.hpp>
#include <helmetEngine/modules/2D/audio.hpp>
#include <helmetEngine/modules/2D/collision.hpp>
#include <helmetEngine/modules/2D/shapes.hpp>
#include <helmetEngine/util/logger.hpp>
#include <helmetEngine/util/uniqueHelper.hpp>

bool Logger::enabled = true;
bool Profiler::enabled = true;

void process(Engine* engine, double deltaTime) {
  static auto* inputModule = engine->getModule<InputModule>("InputModule");
  static auto* audioModule = engine->getModule<AudioModule>("AudioModule");
  static auto* collisionModule = engine->getModule<CollisionModule>("CollisionModule");

  static auto* player = engine->scene->getObject<PhysicsObject>("player");
  static auto* playerSprite = engine->scene->getObject<Sprite>("playerSprite");
  
  double speed = 50;
  double jumpPower = -500;
  bool canJump = true;

  double gravity = 500;
  double gravityAccel = 0.1;

  player->velocity.x *= 0.9;

  if (inputModule->isKeyDown(SDLK_D)) {
    player->velocity.x += speed;
  }
  if (inputModule->isKeyDown(SDLK_A)) {
    player->velocity.x += -speed;
  }
  if (inputModule->isKeyDown(SDLK_SPACE)) {
    if (collisionModule->whatsCollidingWith("player") == "staticObject") {
      player->velocity.y = jumpPower;
    }
  }
  if (inputModule->isKeyDown(SDLK_F)) {
    if (!audioModule->isChannelPlaying(0)) {
      audioModule->playSFX(0, "../resources/sample.wav", 0);
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

  if (player->velocity.y < gravity && !(collisionModule->whatsCollidingWith("player") == "staticObject")){
    player->velocity.y += gravityAccel;
  }
}

int main() {
  Scene scene;


  auto player = std::make_unique<PhysicsObject>();
  player->label = "player";
  player->size = {128, 128};
  player->position = {0, 0};


  auto playerSprite = std::make_unique<Sprite>();
  playerSprite->label = "playerSprite";
  playerSprite->position = {0, 0};
  playerSprite->localPosition = {0, 0};
  playerSprite->size = {128, 128};
  playerSprite->texture = "../resources/icon.png";

  player->addObject(playerSprite.get());


  auto playerCamera = std::make_unique<Camera>();
  playerCamera->label = "playerCamera";
  playerCamera->position = {0, 0};
  playerCamera->localPosition = {0, 0};
  playerCamera->offset = {500 - (playerSprite->size.x / 2), 500 - (playerSprite->size.x / 2)};

  player->addObject(playerCamera.get());

  
  auto staticObj = std::make_unique<PhysicsObject>();
  staticObj->label = "staticObject";
  staticObj->size = {1000, 500};
  staticObj->position = {200, 500};
  staticObj->velocity = {0, 0};
  staticObj->isStatic = true;


  auto staticSprite = std::make_unique<Sprite>();
  staticSprite->label = "staticSprite";
  staticSprite->position = staticObj->position;
  staticSprite->size = {1000, 500};
  staticSprite->texture = "../resources/icon.png";

  staticObj->addObject(staticSprite.get());

  scene.addObject(std::move(playerSprite));
  scene.addObject(std::move(player));
  scene.addObject(std::move(staticSprite));
  scene.addObject(std::move(staticObj));

  Engine engine;


  engine.engineInit(process, &scene,
  make_unique_vector<Module>(
    std::make_unique<RendererModule>("Sprite Test", 1000, 1000, playerCamera.get()),
    std::make_unique<AudioModule>(),
    std::make_unique<InputModule>(),
    std::make_unique<CollisionModule>(4, 4)
    )
  );

  return 0;
}