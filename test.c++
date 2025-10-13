#include "engine.hpp"
#include "scene.hpp"
#include "modules/input.hpp"
#include "modules/renderer.hpp"
#include <iostream>

void process(Engine* engine, double deltaTime) {
  InputModule* inputModule = dynamic_cast<InputModule*>(*engine->findModule<InputModule>());

  Sprite* player = dynamic_cast<Sprite*>(engine->scene->findObject("player")->get());
  
  double speed = 0.05;

  if (inputModule->input->isKeyDown(SDLK_W)) {
    player->position.y -= speed;
  }
  if (inputModule->input->isKeyDown(SDLK_S)) {
    player->position.y += speed;
  }
  if (inputModule->input->isKeyDown(SDLK_D)) {
    player->position.x += speed;
  }
  if (inputModule->input->isKeyDown(SDLK_A)) {
    player->position.x -= speed;
  }
}

int main() {
  Scene scene;


  Renderer renderer;
  renderer.rendererInit("Sprite Test", 1000, 1000);


  auto sprite = std::make_unique<Sprite>();
  sprite->id = "player";
  sprite->position = {0, 0};
  sprite->size = {128, 128};
  sprite->texture = IMG_LoadTexture(renderer.SDLRenderer, "sample.png");

  scene.addObject(std::move(sprite));
  

  RendererModule rendererModule(&renderer);

  Input input;
  InputModule inputModule(&input);


  Engine engine;

  engine.engineInit(process, &scene, {&rendererModule, &inputModule});

  return 0;
}
