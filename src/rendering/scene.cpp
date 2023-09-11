#include "scene.hpp"
#include "screen.hpp"
#include <stdio.h>

Scene::Scene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible)
    : visible_(visible)
    , screen_(screen)
    , rect_on_screen_(rect)
    , surface_(surface)
{
}

void Scene::addToScene(RenderablePtr object) {
    if (object->scene_) {
        throw std::runtime_error("RendererObject is already part of a scene.");
    }
    auto layer = object->layer_;
    if (render_objects_.count(layer) == 0) {
        render_objects_[layer] = std::vector<RenderablePtr>();
    }
    object->setScene(this);
    object->scene_ = this;
    render_objects_[layer].push_back(object);
}

void Scene::removeFromScene(Renderable* object) {
}

void Scene::removeFromScene(RenderablePtr object) {
    if (object.isValid()) {
        auto layer = object->layer_;
        if (render_objects_.count(layer) > 0) {
            std::vector<RenderablePtr>& objects = render_objects_[layer];
            int size = objects.size();
            for (int i = 0; i < size; i++) {
                if (objects[i].isValid() && objects[i].get() == object.get()) {
                    objects.erase(objects.begin() + i);
                    size--;
                }
            }
        }
    }
}

void Scene::setPixel(int x, int y, Uint16 color) {
    if (x >= 0 && y >= 0 && x < surface_->w && y < surface_->h) {
        *((Uint16*)surface_->pixels + y * surface_->pitch + x * surface_->format->BytesPerPixel) = color;
    }
}

void Scene::fillColor(Rect rect, Uint16 color) {
    SDL_Rect rect2{rect.toSDLRect()};
    SDL_FillRect(surface_, &rect2, color);
}

void Scene::drawRect(Rect rect, Uint16 color, Uint8 thickness, Uint8 alpha) {
    Sint16 x = rect.origin_.x_, y = rect.origin_.y_;
    Uint16 width = rect.width_, height = rect.height_;

    SDL_Rect rect0{(Sint16)(x), (Sint16)(y), thickness, (Uint16)(height - thickness)};                                 // left margin
    SDL_Rect rect1{(Sint16)(x + thickness), (Sint16)(y), (Uint16)(width - thickness), (Uint16)(thickness)};                      // top margin
    SDL_Rect rect2{(Sint16)(x + width - thickness), (Sint16)(y + thickness), thickness, (Uint16)(height - thickness)}; // right margin
    SDL_Rect rect3{(Sint16)(x), (Sint16)(y + height - thickness), (Uint16)(width - thickness), (Uint16)(thickness)};             // bottom margin

    SDL_FillRect(surface_, &rect0, color);
    SDL_FillRect(surface_, &rect1, color);
    SDL_FillRect(surface_, &rect2, color);
    SDL_FillRect(surface_, &rect3, color);
}

void Scene::render(RReader<Screen> screen) {
    std::vector<RendererObject> v;
    for (auto vect : render_objects_) {
        auto& vector = vect.second;
        for (auto iter = vector.begin(); iter != vector.end();) {
            if ((*iter).isValid()) {
                (*iter)->render(this);
                iter++;
            } else {
                iter = vector.erase(iter);
            }
        }
    }
    SDL_BlitSurface(surface_, NULL, screen->getSurface(), &rect_on_screen_);
}

Scene::~Scene() {
    SDL_free(surface_);
}
