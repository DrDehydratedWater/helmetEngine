#include "engine.hpp"
#include "renderer.hpp"
#include "scene.hpp"

void process(Scene* scene, double deltaTime) {
    for (auto &object : scene->objects) {
        object->position.x += 0.1 * deltaTime;
    }
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
    sprite->texture = IMG_LoadTexture(renderer.SDLRenderer, "sample.png");

    // Add sprite to the scene
    scene.addObject(std::move(sprite));

    // Create the renderer module
    RendererModule rendererModule(&renderer);

    // Create engine
    Engine engine;

    // Initialize engine
    engine.engineInit(process, &scene, {&rendererModule});
    
    return 0;
}
