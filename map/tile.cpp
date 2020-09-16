#include "tile.hpp"
#include<stdio.h>
#include "../primitives/color_conversion.hpp"
#include "../rendering/renderer.hpp"

Tile::Tile(int x, int y, int width, int height, int index_x, int index_y)
    : RendererObject(x, y, width,height)
    , index_x_(index_x)
    , index_y_(index_y)
    , color_(0x0)
    , type(TileType::GRASS)
{
    updateType(GRASS);
}

void Tile::render(Renderer* renderer){
    renderer->fill_color(RendererObject::boundingBox(), color_);
}

void Tile::updateType(TileType newType){
    type = newType;
    Uint32 effective_color = 0x00c300;
    switch (newType)
    {
    case GRASS:
    case FILLED_GRASS:
        effective_color -= index_x_ % 2 * 0x1300;
        effective_color -= index_y_ % 2 * 0x1300;
        break;
    case PATH:
        effective_color = 0x0000ff;
        break;
    case START:
        effective_color = 0x003300;
        break;
    case FINISH:
        effective_color = 0x330000;
        break;
    default:
        break;
    }
    //888 to 565
    color_ = RGB_888_TO_565(effective_color);
}