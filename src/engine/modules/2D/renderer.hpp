#pragma once

#include "../../scene.hpp"
#include "../../engine.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include "shapes.hpp"
#include "../../util/math/vec2.hpp"

class Sprite : public Rect {
public:
  std::string texture;
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

  using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
  std::vector<std::pair<std::string, TexturePtr>> loadedTextures;
  
  bool rendererInit(const char *title, int width, int height) {
    SDLWindow = SDL_CreateWindow(title, width, height, 0);

    SDLRenderer = SDL_CreateRenderer(SDLWindow, 0);

    SDL_SetWindowIcon(SDLWindow, IMG_Load("../resources/icon.png"));

    return true;
  }

  void drawSprite(const Sprite &sprite) {
    auto it = std::find_if(loadedTextures.begin(), loadedTextures.end(),
      [&](const auto &p){ return p.first == sprite.texture; });

    if (it == loadedTextures.end()) {
      SDL_Texture* rawTexture = IMG_LoadTexture(SDLRenderer, sprite.texture.c_str());
      loadedTextures.emplace_back(sprite.texture, TexturePtr(rawTexture, SDL_DestroyTexture));
      it = std::prev(loadedTextures.end());
    }

    SDL_Texture* texture = it->second.get();

    SDL_FRect dst;
    dst.x = sprite.position.x;
    dst.y = sprite.position.y;
    dst.w = sprite.size.x;
    dst.h = sprite.size.y;

    SDL_RenderTexture(SDLRenderer, texture, nullptr, &dst);
  }

  void shutdownRenderer() {
    loadedTextures.clear();
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