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
    , rect_(x, y, width, height)
{

}

RendererObject::RendererObject(SCREEN_LAYER layer)
    : RendererObject(Rect(), layer)
{

}

RendererObject::RendererObject(Rect rect)
    : Renderable()
    , rect_(std::move(rect))
{

}

RendererObject::RendererObject(Point pos, int width, int height)
    : RendererObject(Rect(pos, width, height)) {}

RendererObject::~RendererObject() {
}

Rect RendererObject::boundingBox() const {
    return rect_;
}

Uint16 RendererObject::getWidth() const {
    return rect_.width_;
}

Uint16 RendererObject::getHeight() const {
    return rect_.height_;
}

Point RendererObject::getCenter() const {
    return rect_.center();
}
