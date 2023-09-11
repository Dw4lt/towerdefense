#include "renderer_object.hpp"

#include "assert.h"
#include "scene.hpp"


void DrawUtils::setPixel(SDL_Surface* surface,int x, int y, Uint16 color) {
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
        *((Uint16*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel) = color;
    }
}

void DrawUtils::fillColor(SDL_Surface* surface,Rect rect, Uint16 color) {
    SDL_Rect rect2{rect.toSDLRect()};
    SDL_FillRect(surface, &rect2, color);
}

void DrawUtils::drawRect(SDL_Surface* surface, Rect rect, Uint16 color, Uint8 thickness) {
    Sint16 x = rect.origin_.x_, y = rect.origin_.y_;
    Uint16 width = rect.width_, height = rect.height_;

    SDL_Rect rect0{(Sint16)(x), (Sint16)(y), thickness, (Uint16)(height - thickness)};                                 // left margin
    SDL_Rect rect1{(Sint16)(x + thickness), (Sint16)(y), (Uint16)(width - thickness), (Uint16)(thickness)};                      // top margin
    SDL_Rect rect2{(Sint16)(x + width - thickness), (Sint16)(y + thickness), thickness, (Uint16)(height - thickness)}; // right margin
    SDL_Rect rect3{(Sint16)(x), (Sint16)(y + height - thickness), (Uint16)(width - thickness), (Uint16)(thickness)};             // bottom margin

    SDL_FillRect(surface, &rect0, color);
    SDL_FillRect(surface, &rect1, color);
    SDL_FillRect(surface, &rect2, color);
    SDL_FillRect(surface, &rect3, color);
}


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

void Renderable::renderChildren(SDL_Surface* surface) {
    for (auto child : children_) {
        child->render(surface);
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
