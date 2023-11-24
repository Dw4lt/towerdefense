#include "structure.hpp"
#include "../rendering/composable_scene.hpp"
#include "../util/color.hpp"

Structure::Structure(const Rect& bounding_box)
    : RendererObject(bounding_box, SCREEN_LAYER::STRUCTURE)
{
}

Structure::~Structure() {}

void Structure::render(SDL_Surface* surface) {
    DrawUtils::fillColor(surface, boundingBox(), Colors::UNKNOWN);
    RendererObject::renderChildren(surface);
}
