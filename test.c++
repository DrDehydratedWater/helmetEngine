#include "engine.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <memory>

void process(Scene* scene) {

}

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

    Engine engine;

    std::vector<Module*> modules = {&rendererModule};

    engine.engineInit(process, &scene, modules);

    renderer.shutdownRenderer();
    return 0;
}
