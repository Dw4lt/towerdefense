#include "abstract_scene.hpp"
#include "screen.hpp"

AbstractScene::AbstractScene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible)
    : visible_(visible)
    , screen_(screen)
    , rect_on_screen_(std::move(rect))
    , background_surface_(surface)
{
}

AbstractScene::~AbstractScene() {
    SDL_free(background_surface_);
}
