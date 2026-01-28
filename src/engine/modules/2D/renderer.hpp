#pragma once

#include "../../scene.hpp"
#include "../../engine.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include "shapes.hpp"
#include "../../util/math/vec2.hpp"

/// @brief Sprite that stores the texture path as a string
class Sprite : public Rect {
public:
  std::string texture;
  bool visible = true;
};

class AnimatedSprite : public Rect {
public:
  bool visible = true;
  std::vector<std::unique_ptr<Sprite>> frames;
  int frame = 0;
  double speed = 0.1;
  double timePassed = 0;

  void setSprite(const std::string& path) {
    std::vector<std::filesystem::path> files;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      if (entry.is_regular_file()) {
        files.push_back(entry.path());
      }
    }

    std::sort(files.begin(), files.end(),
      [](const std::filesystem::path& a, const std::filesystem::path& b) {
          return std::stoi(a.stem().string()) < std::stoi(b.stem().string());
      }
    );
    
    for (const auto& file : files) {
      auto sprite = std::make_unique<Sprite>();
      sprite->texture = file.string();
      sprite->position = position;
      sprite->localPosition = localPosition;
      sprite->size = size;
      sprite->visible = visible;
      frames.push_back(std::move(sprite));
    }
  }
};

/// @brief Camera, each sprite is placed on the window from the relative position of the sprite from the camera
class Camera : public Rect {
public:
  Vec2 offset;
};

/// @brief Renders sprites
class RendererModule : public Module {
private:
  Engine* engine;

  SDL_Renderer *SDLRenderer = NULL;
  SDL_Window *SDLWindow = NULL;

  using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
  std::vector<std::pair<std::string, TexturePtr>> loadedTextures;
  
  bool rendererInit(const char *title, int width, int height, Camera *camera_) {
    SDLWindow = SDL_CreateWindow(title, width, height, 0);
    SDLRenderer = SDL_CreateRenderer(SDLWindow, 0);

    camera = camera_;

    return true;
  }

  // Texture management is already covered here thus it works with animated sprites too
  void drawSprite(const Sprite &sprite) {
    auto it = std::find_if(loadedTextures.begin(), loadedTextures.end(),
      [&](const auto &p){ return p.first == sprite.texture; });

    if (it == loadedTextures.end()) {
      SDL_Texture* rawTexture = IMG_LoadTexture(SDLRenderer, sprite.texture.c_str());
      loadedTextures.emplace_back(sprite.texture, TexturePtr(rawTexture, SDL_DestroyTexture));
      it = std::prev(loadedTextures.end());
    }

    SDL_Texture* texture = it->second.get();

    SDL_FRect rect;
    rect.x = sprite.position.x - camera->position.x + camera->offset.x;
    rect.y = sprite.position.y - camera->position.y + camera->offset.y;
    rect.w = sprite.size.x;
    rect.h = sprite.size.y;

    SDL_RenderTexture(SDLRenderer, texture, nullptr, &rect);
  }

  void shutdownRenderer() {
    loadedTextures.clear();
    SDL_Quit();
  }

  void startup(Engine* temp) override {
    engine = temp;
  }

  void main() override {
    SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(SDLRenderer);

    for (const auto& obj : engine->scene->objects) {
      if (auto sprite = dynamic_cast<Sprite*>(obj.get())) {
        drawSprite(*sprite);
      }

      if (auto animatedSprite = dynamic_cast<AnimatedSprite*>(obj.get())) {
        animatedSprite->timePassed += engine->deltaTime;

        if (animatedSprite->timePassed >= animatedSprite->speed) {
          animatedSprite->frame++;
          if (animatedSprite->frame >= animatedSprite->frames.size()) {
            animatedSprite->frame = 0;
          }
          animatedSprite->timePassed = 0;
        }
        Sprite* frame = animatedSprite->frames.at(animatedSprite->frame).get();
        frame->position = animatedSprite->position;
        frame->localPosition = animatedSprite->localPosition;
        frame->size = animatedSprite->size;
        frame->visible = animatedSprite->visible;
        drawSprite(*frame);
      }
    }
    SDL_RenderPresent(SDLRenderer);
  }

  void shutdown() override {
    shutdownRenderer();
  }

public:
  const Camera* camera;

  RendererModule(const char* title, int width, int height, Camera* camera) 
      : Module("RendererModule") {
    rendererInit(title, width, height, camera);
  }
};