#include "structure.hpp"
#include "../rendering/renderer.hpp"

Structure::Structure(const Tile& tile)
    : RendererObject(tile.boundingBox())
{
}

Structure::~Structure() {}

void Structure::render(Renderer* renderer) {
    renderer->fillColor(boundingBox(), RGB_888_TO_565(0xFF00FF));
    RendererObject::renderChildren(renderer);
}
