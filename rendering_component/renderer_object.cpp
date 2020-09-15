#include "renderer_object.hpp"
#include"../rendering_engine/renderer.hpp"

RendererObject::RendererObject(int x, int y, int width, int height)
    : width_(width)
    , height_(height)
    , x_(x)
    , y_(y)
{

}

SDL_Rect RendererObject::boundingBox(){
    return SDL_Rect{x_, y_, width_, height_};
}