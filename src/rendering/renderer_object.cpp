#include "renderer_object.hpp"

#include "assert.h"
#include "renderer.hpp"

Renderable::Renderable() {
}

auto Renderable::addChild(RReader<Renderable> child) -> void {
    children_.push_back(child);
}

Renderable::~Renderable() {
    if (!children_.empty()) {
        children_.erase(children_.begin(), children_.end());
    }
}

void Renderable::renderChildren(Renderer* renderer) {
    for (auto child : children_) {
        child->render(renderer);
    }
}


// ----

RendererObject::RendererObject(int x, int y, int width, int height)
    : Renderable()
    , x_(x)
    , y_(y)
    , width_(width)
    , height_(height) {
}

RendererObject::RendererObject(const Rect& rect)
    : RendererObject(rect.origin_, rect.width_, rect.height_) {}

RendererObject::RendererObject(Point pos, int width, int height)
    : RendererObject(pos.x_, pos.y_, width, height) {}

RendererObject::~RendererObject() {
}

Rect RendererObject::boundingBox() const {
    return Rect{x_, y_, width_, height_};
}

void RendererObject::render(Renderer* renderer, const Rect& region) {
    render(renderer);
}

Uint16 RendererObject::getWidth() const {
    return width_;
}

Uint16 RendererObject::getHeight() const {
    return height_;
}

Point RendererObject::getCenter() const {
    return Point((int)std::round(x_ + width_ / 2.0),
                 (int)std::round(y_ + height_ / 2.0));
}
