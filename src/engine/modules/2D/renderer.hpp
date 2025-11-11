#pragma once

#include "../../scene.hpp"
#include "../../engine.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include "shapes.hpp"
#include "../../util/math/vec2.hpp"

class Sprite : public Box {
public:
  SDL_Texture *texture;
  bool visible;
};

class RendererModule : public Module {
public:
  RendererModule(const char* title, int width, int height) 
      : Module("RendererModule") {
    rendererInit(title, width, height);
  }

  SDL_Renderer *SDLRenderer = NULL;
  SDL_Window *SDLWindow = NULL;

  
  bool rendererInit(const char *title, int width, int height) {
    SDLWindow = SDL_CreateWindow(title, width, height, 0);

    SDLRenderer = SDL_CreateRenderer(SDLWindow, 0);

    SDL_SetWindowIcon(SDLWindow, IMG_Load("../resources/icon.png"));

    return true;
  }

  void drawSprite(const Sprite &sprite) {
    if (!SDLRenderer || !sprite.texture)
      return;

    SDL_FRect dst;
    dst.x = sprite.position.x;
    dst.y = sprite.position.y;
    dst.w = sprite.size.x;
    dst.h = sprite.size.y;

    SDL_RenderTexture(SDLRenderer, sprite.texture, nullptr, &dst);
  }

  void shutdownRenderer() {
    SDL_Quit();
  }


  void main(Engine* engine) override {
    SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(SDLRenderer);

    for (const auto& obj : engine->scene->objects) {
      if (auto sprite = dynamic_cast<Sprite*>(obj.get())) {
        drawSprite(*sprite);
      }
    }

    SDL_RenderPresent(SDLRenderer);
  }

  void shutdown(Engine* engine) override {
    shutdownRenderer();
  }
};