#include "renderer_object.hpp"

#include "assert.h"
#include "scene.hpp"

Renderable::Renderable() {
}

auto Renderable::addChild(RReader<Renderable> child) -> void {
    children_.push_back(child);
    child->part_of_a_scene |= part_of_a_scene; // Inherit scene if assigned
}

void Renderable::inheritScene() { // TODO: hacky approach. Add proper state management
    for(auto& child: children_) {
        child->part_of_a_scene = part_of_a_scene;
    }
}

Renderable::~Renderable() {
    if (!children_.empty()) {
        children_.erase(children_.begin(), children_.end());
    }
}

void Renderable::renderChildren(Scene* scene) {
    for (auto child : children_) {
        child->render(scene);
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

void RendererObject::render(Scene* scene, const Rect& region) {
    render(scene);
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
