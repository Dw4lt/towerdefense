#include "scene.hpp"
#include <stdio.h>

std::unique_ptr<Scene> Scene::singleton_(nullptr);

auto Scene::Init(int width, int height, int bit_per_color) -> Scene::SceneRef {
    singleton_.reset(new Scene(width, height, bit_per_color));
    return Scene::singleton_;
}

Scene::Scene(int width, int height, int bit_per_color)
    : screen_width_(width)
    , screen_height_(height)
    , screen_bit_color_(bit_per_color)
    , screen_(SDL_SetVideoMode(screen_width_, screen_height_, screen_bit_color_, SDL_FULLSCREEN)) {
}

auto Scene::get() -> Scene::SceneRef {
    if (!singleton_) throw std::runtime_error("Scene requested before initialization");
    return singleton_;
}

void Scene::addToScene(RendererObjectPtr object) {
    if (object->part_of_a_scene) {
        std::cout << "RendererObject is already part of a scene.\n";
        return;
    }
    auto layer = object->getDepth();
    if (render_objects_.count(layer) == 0) {
        render_objects_[layer] = std::vector<RendererObjectPtr>();
    }
    object->part_of_a_scene = true;
    object->inheritScene();
    render_objects_[layer].push_back(object);
}

void Scene::removeFromScene(RendererObjectPtr object) {
    if (object.isValid()) {
        auto layer = object->getDepth();
        if (render_objects_.count(layer) > 0) {
            std::vector<RendererObjectPtr>& objects = render_objects_[layer];
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
    if (x >= 0 && y >= 0 && x < screen_width_ && y < screen_height_) {
        *((Uint16*)screen_->pixels + y * screen_->pitch + x * screen_bit_color_) = color;
    }
}

void Scene::fillColor(Rect rect, Uint16 color) {
    SDL_Rect rect2{rect.toSDLRect()};
    SDL_FillRect(screen_, &rect2, color);
}

void Scene::drawRect(Rect rect, Uint16 color, Uint8 thickness, Uint8 alpha) {
    Sint16 x = rect.origin_.x_, y = rect.origin_.y_;
    Uint16 width = rect.width_, height = rect.height_;

    SDL_Rect rect0{(Sint16)(x), (Sint16)(y), thickness, (Uint16)(height - thickness)};                                 // left margin
    SDL_Rect rect1{(Sint16)(x + thickness), (Sint16)(y), (Uint16)(width - thickness), (Uint16)(thickness)};                      // top margin
    SDL_Rect rect2{(Sint16)(x + width - thickness), (Sint16)(y + thickness), thickness, (Uint16)(height - thickness)}; // right margin
    SDL_Rect rect3{(Sint16)(x), (Sint16)(y + height - thickness), (Uint16)(width - thickness), (Uint16)(thickness)};             // bottom margin

    SDL_FillRect(screen_, &rect0, color);
    SDL_FillRect(screen_, &rect1, color);
    SDL_FillRect(screen_, &rect2, color);
    SDL_FillRect(screen_, &rect3, color);
}

void Scene::render() {
    std::vector<RendererObject> v;
    for (auto vect : render_objects_) {
        for (auto obj : vect.second) {
            if (obj.isValid()) {
                obj->render(this);
            } else {
                // TODO: Remove from scene
            }
        }
    }
}

Scene::~Scene() {
    SDL_free(screen_);
}

void Scene::show() {
    SDL_Flip(screen_);
}
