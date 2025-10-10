#include "engine.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <memory>

int main() {
    // Create the scene
    Scene scene;

    // Create the renderer
    Renderer renderer;
    renderer.rendererInit("Sprite Test", 400, 400);

    // Load the sprite
    auto sprite = std::make_unique<Sprite>();
    sprite->id = "sprite1";
    sprite->position = {0, 0};
    sprite->size = {64, 64}; // Set a default size
    sprite->texture = IMG_LoadTexture(renderer.renderer, "sample.png");

    // Add sprite to the scene
    scene.addObject(std::move(sprite));

    // Create the renderer module
    RendererModule rendererModule(&renderer);

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        rendererModule.main(&scene);
        SDL_Delay(16); // ~60 FPS
    }

    renderer.shutdownRenderer();
    return 0;
}
