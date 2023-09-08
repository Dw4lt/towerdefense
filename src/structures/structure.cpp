#include "structure.hpp"
#include "../rendering/scene.hpp"

Structure::Structure(const Tile& tile)
    : RendererObject(tile.boundingBox())
{
}

Structure::~Structure() {}

void Structure::render(Scene* scene) {
    scene->fillColor(boundingBox(), RGB_888_TO_565(0xFF00FF));
    RendererObject::renderChildren(scene);
}
