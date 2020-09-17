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

RendererObject::RendererObject(Point pos, int width, int height)
    : RendererObject(pos.x_, pos.y_, width, height)
{}

RendererObject::~RendererObject(){
    if (!children_.empty()) {
        children_.erase(children_.begin(), children_.end());
    }
}

Rect RendererObject::boundingBox() const{
    return Rect{x_, y_, width_, height_};
}

void RendererObject::render(Renderer* renderer, const Rect& region){
    render(renderer);
}

Point RendererObject::getCenter(){
    return Point((int) std::round(x_ + width_/2.0),
                 (int) std::round(y_ + height_/2.0));
}