#include "tile.hpp"
#include "../primitives/color_conversion.hpp"
#include "../rendering/scene.hpp"
#include <stdio.h>

Tile::Tile(int x, int y, int width, int height, int index_x, int index_y)
    : RendererObject(x, y, width, height)
    , index_x_(index_x)
    , index_y_(index_y)
    , color_(0x0)
    , type_(TileType::LAND)
    , next_neighbour_(Direction::NONE)
    , terrain_(TileTerrain::TREE) {
    updateColor();
}

void Tile::render(Scene* scene) {
    scene->fillColor(RendererObject::boundingBox(), color_);
    RendererObject::renderChildren(scene);
}

void Tile::updateType(TileType newType) {
    type_ = newType;
    updateColor();
}

void Tile::updateNextNeighbour(Direction direction) {
    next_neighbour_ = direction;
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
            effective_color = ((PSEUDO_RANDOM_SEEDED(rect_.origin_.x_, rect_.origin_.y_) % 6) * 3 + 0x75) << 8; // MAGIC
            break;
        case TileTerrain::ROCK:
            break;
        default:
            break;
        }
        break;
    case TileType::PATH:
        effective_color = 0x0000ff;
        break;
    default:
        break;
    }
    color_ = RGB_888_TO_565(effective_color);
}
