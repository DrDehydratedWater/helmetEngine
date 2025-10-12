#include "engine.hpp"
#include "scene.hpp"
#include "modules/input.hpp"
#include "modules/renderer.hpp"
#include <iostream>

void process(Engine* engine, double deltaTime) {
  Input* input = nullptr;

  for (Module* mod : engine->modules) {
    if (auto im = dynamic_cast<InputModule*>(mod)) {
      input = im->input;
      break;
    }
  }

  Sprite* player;

  auto it = engine->scene->findObject("player");
  if (it != engine->scene->objects.end()) {
    player = dynamic_cast<Sprite*>(it->get());
    if (!player) {
        std::cout << "Player not found!";
    }
  }

  if (input->isKeyDown(SDLK_W)) {
    player->position.x += 0.05;
  }
}

int main() {
  Scene scene;


  Renderer renderer;
  renderer.rendererInit("Sprite Test", 400, 400);


  auto sprite = std::make_unique<Sprite>();
  sprite->id = "player";
  sprite->position = {0, 0};
  sprite->size = {64, 64};
  sprite->texture = IMG_LoadTexture(renderer.SDLRenderer, "sample.png");

  scene.addObject(std::move(sprite));
  

  RendererModule rendererModule(&renderer);

  Input input;
  InputModule inputModule(&input);


  Engine engine;

  engine.engineInit(process, &scene, {&rendererModule, &inputModule});

  return 0;
}
