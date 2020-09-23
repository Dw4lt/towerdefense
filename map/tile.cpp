#include "tile.hpp"
#include<stdio.h>
#include "../primitives/color_conversion.hpp"
#include "../rendering/renderer.hpp"

Tile::Tile(int x, int y, int width, int height, int index_x, int index_y)
    : RendererObject(x, y, width,height)
    , index_x_(index_x)
    , index_y_(index_y)
    , color_(0x0)
    , type_(TileType::LAND)
    , next_neighbour_(Direction::NONE)
    , terrain_(TileTerrain::TREE)
{
    updateColor();
}

void Tile::render(Renderer* renderer){
    renderer->fill_color(RendererObject::boundingBox(), color_);
}

void Tile::updateType(TileType newType){
    type_ = newType;
    updateColor();
}

void Tile::updateNextNeighbour(Direction direction){
    next_neighbour_ = direction;
}

void Tile::updateDistanceToWater(unsigned int distance){
    distance_to_water_ = distance;
    if (distance > 5){
        updateTerrain(TileTerrain::TREE);
    }
}

void Tile::updateTerrain(TileTerrain terrain){
    terrain_ = terrain;
    updateColor();
}

void Tile::updateColor(){
    Uint32 effective_color = 0x00c300;
    switch (type_)
    {
    case LAND:
        switch (terrain_)
        {
        case TileTerrain::PLAIN:
            effective_color -= index_x_ % 2 * 0x1300;
            effective_color -= index_y_ % 2 * 0x1300;
            break;
        case TileTerrain::TREE:
            effective_color = ((PSEUDO_RANDOM_SEEDED(x_, y_) % 6) * 3 + 0x75) << 8; // MAGIC
            break;
        case TileTerrain::ROCK:
            break;
        default:
            break;
        }
        break;
    case PATH:
        effective_color = 0x0000ff;
        break;
    default:
        break;
    }
    color_ = RGB_888_TO_565(effective_color);
}