#include "structure.hpp"
#include "../rendering/composable_scene.hpp"

Structure::Structure(const Rect& bounding_box)
    : RendererObject(bounding_box, SCREEN_LAYER::STRUCTURE)
{
}

Structure::~Structure() {}

void Structure::render(SDL_Surface* surface) {
    DrawUtils::fillColor(surface, boundingBox(), RGB_888_TO_565(0xFF00FF));
    RendererObject::renderChildren(surface);
}
