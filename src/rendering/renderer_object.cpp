#include "renderer_object.hpp"

#include "assert.h"
#include "scene.hpp"

Renderable::Renderable(SCREEN_LAYER layer)
    : scene_{nullptr}
    , layer_{layer}
{
}

auto Renderable::addChild(RReader<Renderable> child) -> void {
    children_.push_back(child);
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

SCREEN_LAYER Renderable::getLayer(){
    return layer_;
}

void Renderable::setScene(Scene* scene) {
    scene_ = scene;
}


// ----

RendererObject::RendererObject(int x, int y, int width, int height, SCREEN_LAYER layer)
    : Renderable(layer)
    , rect_(x, y, width, height)
{

}

RendererObject::RendererObject(SCREEN_LAYER layer)
    : RendererObject(Rect(), layer)
{

}

RendererObject::RendererObject(Rect rect, SCREEN_LAYER layer)
    : Renderable(layer)
    , rect_(std::move(rect))
{

}

RendererObject::RendererObject(Point pos, int width, int height, SCREEN_LAYER layer)
    : RendererObject(Rect(pos, width, height), layer) {}

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
