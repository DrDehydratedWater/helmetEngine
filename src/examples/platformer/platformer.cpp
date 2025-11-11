#include <helmetEngine/engine.hpp>
#include <helmetEngine/scene.hpp>
#include <helmetEngine/modules/2D/input.hpp>
#include <helmetEngine/modules/2D/renderer.hpp>
#include <helmetEngine/modules/2D/audio.hpp>
#include <helmetEngine/modules/2D/collision.hpp>
#include <helmetEngine/util/logger.hpp>
#include <helmetEngine/util/uniqueHelper.hpp>

bool Logger::enabled = true;
bool Profiler::enabled = true;

void process(Engine* engine, double deltaTime) {
  static auto* inputModule = engine->getModule<InputModule>("InputModule");
  static auto* audioModule = engine->getModule<AudioModule>("AudioModule");

  static auto* player = engine->scene->getObject<PhysicsObject>("player");
  static auto* playerSprite = engine->scene->getObject<Sprite>("playerSprite");
  
  double speed = 150;
  double jumpPower = -0.1;
  bool canJump = true;

  double gravity = 100;
  double gravityAccel = 0.01;

  std::cout << "Player velocity: " << player->velocity << "\n" << "Player position: " << player->position << "\n";

  if (inputModule->isKeyDown(SDLK_D)) {
    player->velocity.x = speed;
  }
  if (inputModule->isKeyDown(SDLK_A)) {
    player->velocity.x = -speed;
  }
  if (inputModule->isKeyDown(SDLK_SPACE)) {
    player->velocity.y += jumpPower;
  }
  if (inputModule->isKeyDown(SDLK_F)) {
    if (!audioModule->isChannelPlaying(0)) {
      audioModule->playSFX(0, "../resources/sample.wav", 1);
    }
    playerSprite->size += {0.005, 0.005};
    player->box->size += {0.005, 0.005};
  }
  if (inputModule->isKeyDown(SDLK_G)) {
    playerSprite->size -= {0.005, 0.005};
    player->box->size -= {0.005, 0.005};
  }
  if (inputModule->isKeyDown(SDLK_Q)) {
    engine->shutdown();
  }

  if (player->velocity.y < gravity){
    player->velocity.y += gravityAccel;
  }
}

int main() {
  auto rendererModule = std::make_unique<RendererModule>("Sprite Test", 1000, 1000);

  Scene scene;


  auto player = std::make_unique<PhysicsObject>();
  player->box = std::make_unique<Box>();
  player->id = "player";
  player->box->size = {128, 128};
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
  staticObj->box = std::make_unique<Box>();
  staticObj->box->size = {0, 0};
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
    std::make_unique<AudioModule>(),
    std::make_unique<InputModule>(),
    std::make_unique<CollisionModule>()
    )
  );

  return 0;
}