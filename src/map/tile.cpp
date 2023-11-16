#include "tile.hpp"
#include "../primitives/color_conversion.hpp"
#include "../rendering/composable_scene.hpp"
#include <stdio.h>

Tile::Tile(int x, int y, int width, int height, int index_x, int index_y)
    : RendererObject(x, y, width, height, SCREEN_LAYER::BACKGROUND)
    , index_x_(index_x)
    , index_y_(index_y)
    , color_(0x0)
    , type_(TileType::LAND)
    , terrain_(TileTerrain::TREE) {
    updateColor();
}

void Tile::render(SDL_Surface* surface) {
    DrawUtils::fillColor(surface, RendererObject::boundingBox(), color_);
    RendererObject::renderChildren(surface);
}

void Tile::updateType(TileType newType) {
    type_ = newType;
    updateColor();
}

void Tile::updateTerrain(TileTerrain terrain) {
    terrain_ = terrain;
    updateColor();
}

void Tile::updateColor() {
    Uint32 effective_color = 0x00c300;
    switch (type_) {
    case TileType::LAND:
        switch (terrain_) {
        case TileTerrain::PLAIN:
            effective_color -= index_x_ % 2 * 0x1300;
            effective_color -= index_y_ % 2 * 0x1300;
            break;
        case TileTerrain::TREE:
            effective_color = ((PSEUDO_RANDOM_SEEDED(index_x_, index_y_) % 6) * 3 + 0x75) << 8; // MAGIC
            break;
        case TileTerrain::ROCK:
            break;
        default:
            break;
        }
        break;
    case TileType::PATH:
        effective_color = 0x8B4513;
        break;
    default:
        break;
    }
    color_ = RGB_888_TO_565(effective_color);
}
