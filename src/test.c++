#include "engine.hpp"
#include "scene.hpp"
#include "modules/input.hpp"
#include "modules/renderer.hpp"
#include "modules/audio.hpp"
#include <iostream>

void process(Engine* engine, double deltaTime) {
  InputModule* inputModule = dynamic_cast<InputModule*>(*engine->findModule<InputModule>());

  Sprite* player = dynamic_cast<Sprite*>(engine->scene->findObject("player")->get());

  AudioModule* audioModule = dynamic_cast<AudioModule*>(*engine->findModule<AudioModule>());
  
  double speed = 0.05;

  if (inputModule->isKeyDown(SDLK_W)) {
    player->position.y -= speed;
  }
  if (inputModule->isKeyDown(SDLK_S)) {
    player->position.y += speed;
  }
  if (inputModule->isKeyDown(SDLK_D)) {
    player->position.x += speed;
  }
  if (inputModule->isKeyDown(SDLK_A)) {
    player->position.x -= speed;
  }
  if (inputModule->isKeyDown(SDLK_F)) {
    if (audioModule->isPlaying("..resources/sample.wav") == false) {
      audioModule->playAudio("../resources/sample.wav");
    } else {
      audioModule->closeAudio();
    }
    player->size += {0.005, 0.005};
  }
  if (inputModule->isKeyDown(SDLK_G)) {
    player->size -= {0.005, 0.005};
  }
}

int main() {
  RendererModule rendererModule;
  rendererModule.rendererInit("Sprite Test", 1000, 1000);

  AudioModule audioModule;
  audioModule.audioInit();
  
  InputModule inputModule;


  Scene scene;

  auto sprite = std::make_unique<Sprite>();
  sprite->id = "player";
  sprite->position = {0, 0};
  sprite->size = {128, 128};
  sprite->texture = IMG_LoadTexture(rendererModule.SDLRenderer, "../resources/icon.png");

  scene.addObject(std::move(sprite));


  Engine engine;

  engine.engineInit(process, &scene, {&rendererModule, &inputModule, &audioModule});

  return 0;
}
