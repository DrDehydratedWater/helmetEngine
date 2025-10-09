#include "scene.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <dehydrated/math/vector.h>
#include <string>

class Sprite : public gameObject {
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

  void drawGameObject(const Sprite &sprite) {
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
