#include "renderer_object.hpp"
#include"renderer.hpp"

RendererObject::RendererObject(int x, int y, int width, int height)
    : children_()
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height)
{

}

RendererObject::~RendererObject(){
    if (!children_.empty()) {
        children_.erase(children_.begin(), children_.end());
    }
}

SDL_Rect RendererObject::boundingBox(){
    return SDL_Rect{x_, y_, width_, height_};
}

void RendererObject::render(Renderer* renderer, const SDL_Rect& region){
    render(renderer);
}