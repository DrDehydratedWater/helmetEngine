#pragma once

#include "scene.hpp"
#include "engine.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <dehydrated/math/vector.h>

class Sprite : public GameObject {
public:
  SDL_Texture *texture;
  Vec2 size;
};

class Renderer {
public:
  SDL_Renderer *renderer = NULL;
  SDL_Window *window = NULL;

  bool rendererInit(const char *title, int width, int height) {
    window = SDL_CreateWindow(title, width, height, 0);

    renderer = SDL_CreateRenderer(window, 0);

    return true;
  }

  void drawSprite(const Sprite &sprite) {
    if (!renderer || !sprite.texture)
      return;

    SDL_FRect dst;
    dst.x = sprite.position.x;
    dst.y = sprite.position.y;
    dst.w = sprite.size.x;
    dst.h = sprite.size.x;

    SDL_RenderTexture(renderer, sprite.texture, nullptr, &dst);
  }

  void shutdownRenderer() { SDL_Quit(); }
};

class RendererModule : public Module {
public:
  Renderer* renderer; // Set this to your renderer instance

  RendererModule(Renderer* r) : renderer(r) {}

  void main(Scene* scene) override {
    SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer->renderer);

    for (const auto& obj : scene->objects) {
      if (auto sprite = dynamic_cast<Sprite*>(obj.get())) {
        renderer->drawSprite(*sprite);
      }
    }

    SDL_RenderPresent(renderer->renderer);
  }
};