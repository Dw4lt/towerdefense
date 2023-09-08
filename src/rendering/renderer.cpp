#include "renderer.hpp"
#include <stdio.h>

std::unique_ptr<Renderer> Renderer::singleton_(nullptr);

auto Renderer::Init(int width, int height, int bit_per_color) -> Renderer::RendererRef {
    singleton_.reset(new Renderer(width, height, bit_per_color));
    return Renderer::singleton_;
}

Renderer::Renderer(int width, int height, int bit_per_color)
    : screen_width_(width)
    , screen_height_(height)
    , screen_bit_color_(bit_per_color)
    , screen_(SDL_SetVideoMode(screen_width_, screen_height_, screen_bit_color_, SDL_FULLSCREEN)) {
}

auto Renderer::get() -> Renderer::RendererRef {
    if (!singleton_) throw std::runtime_error("Renderer requested before initialization");
    return singleton_;
}

void Renderer::addToScene(RendererObjectPtr object) {
    if (object->part_of_a_scene) {
        std::cout << "RendererObject is already part of a scene.\n";
        return;
    }
    auto layer = object->getDepth();
    if (render_objects_.count(layer) == 0) {
        render_objects_[layer] = std::vector<RendererObjectPtr>();
    }
    object->part_of_a_scene = true;
    render_objects_[layer].push_back(object);
    object->part_of_a_scene = true;
}

void Renderer::removeFromScene(RendererObjectPtr object) {
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

void Renderer::setPixel(int x, int y, Uint16 color) {
    if (x >= 0 && y >= 0 && x < screen_width_ && y < screen_height_) {
        *((Uint16*)screen_->pixels + y * screen_->pitch + x * screen_bit_color_) = color;
    }
}

void Renderer::fillColor(Rect rect, Uint16 color) {
    SDL_Rect rect2{rect.toSDLRect()};
    SDL_FillRect(screen_, &rect2, color);
}

void Renderer::drawRect(Rect rect, Uint16 color, unsigned int thickness, Uint8 alpha) {
    int x = rect.origin_.x_, y = rect.origin_.y_;
    int width = rect.width_, height = rect.height_;
    SDL_Rect rect0{Rect(rect.origin_, thickness, rect.height_ - thickness).toSDLRect()};                          // left margin
    SDL_Rect rect1{Rect(Point(x + thickness, y), width - thickness, thickness).toSDLRect()};                      // Top  Margin
    SDL_Rect rect2{Rect(Point(x + width - thickness, y + thickness), thickness, height - thickness).toSDLRect()}; // Right  Margin
    SDL_Rect rect3{Rect(Point(x, y + height - thickness), width - thickness, thickness).toSDLRect()};             // Bottom  Margin
    SDL_FillRect(screen_, &rect0, color);
    SDL_FillRect(screen_, &rect1, color);
    SDL_FillRect(screen_, &rect2, color);
    SDL_FillRect(screen_, &rect3, color);
}

void Renderer::render() {
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

Renderer::~Renderer() {
    SDL_free(screen_);
}

void Renderer::show() {
    SDL_Flip(screen_);
}
