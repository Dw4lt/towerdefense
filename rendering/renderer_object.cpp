#include "renderer_object.hpp"
#include"renderer.hpp"
#include"assert.h"

RendererObject::RendererObject(int x, int y, int width, int height)
    : children_()
    , parent_(nullptr)
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height)
{
}

RendererObject::RendererObject(const Rect& rect)
    : RendererObject(rect.origin_, rect.width_, rect.height_)
{}

RendererObject::RendererObject(Point pos, int width, int height)
    : RendererObject(pos.x_, pos.y_, width, height)
{}

RendererObject::~RendererObject(){
    if (!children_.empty()) {
        children_.erase(children_.begin(), children_.end());
    }
}

void RendererObject::setParent(RendererObject* parent){
    parent_ = parent;
}

void RendererObject::addChild(RendererObject* child){
    assert(child != nullptr);
    children_.push_back(child);
    child->setParent(this);
}

void RendererObject::renderChildren(Renderer* renderer){
    for (auto child : children_){
        child->render(renderer);
    }
}

Rect RendererObject::boundingBox() const{
    return Rect{x_, y_, width_, height_};
}

void RendererObject::render(Renderer* renderer, const Rect& region){
    render(renderer);
}


Uint16 RendererObject::getWidth() const {
    return width_;
}

Uint16 RendererObject::getHeight() const {
    return height_;
}

Point RendererObject::getCenter() const{
    return Point((int) std::round(x_ + width_/2.0),
                 (int) std::round(y_ + height_/2.0));
}