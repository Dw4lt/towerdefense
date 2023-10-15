#include "renderer_object.hpp"

#include "assert.h"
#include "composable_scene.hpp"


namespace DrawUtils {
    void setPixel(SDL_Surface* surface, int x, int y, Uint16 color) {
        if (x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
            *(
                (Uint16*)(
                    (char*) surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel
                )
            ) = color;
        }
    }

    void fillColor(SDL_Surface* surface, Rect rect, Uint16 color) {
        SDL_Rect rect2{rect.toSDLRect()};
        SDL_FillRect(surface, &rect2, color);
    }

    void drawRect(SDL_Surface* surface, Rect rect, Uint16 color, Uint8 thickness) {
        Sint16 x = rect.origin_.x_, y = rect.origin_.y_;
        Uint16 width = rect.width_, height = rect.height_;

        SDL_Rect rect0{(Sint16)(x), (Sint16)(y), thickness, (Uint16)(height - thickness)};                                 // left margin
        SDL_Rect rect1{(Sint16)(x + thickness), (Sint16)(y), (Uint16)(width - thickness), (Uint16)(thickness)};            // top margin
        SDL_Rect rect2{(Sint16)(x + width - thickness), (Sint16)(y + thickness), thickness, (Uint16)(height - thickness)}; // right margin
        SDL_Rect rect3{(Sint16)(x), (Sint16)(y + height - thickness), (Uint16)(width - thickness), (Uint16)(thickness)};   // bottom margin

        SDL_FillRect(surface, &rect0, color);
        SDL_FillRect(surface, &rect1, color);
        SDL_FillRect(surface, &rect2, color);
        SDL_FillRect(surface, &rect3, color);
    }

    int verticallyCenterFont(nSDL_Font* font, const SDL_Rect& rect) {
        int font_height = nSDL_GetStringHeight(font, "#");
        return rect.y - (font_height / 2.0) + (rect.h / 2.0);
    }

    void drawLineFast(SDL_Surface* surface, float x1, float y1, float x2, float y2, const Uint16 color) {
        if (x2 < x1) std::swap(x1, x2);
        for (int x = x1; x <= x2; x++){
            setPixel(surface, x, y1, color);
        }
        if (y2 < y1) std::swap(y1, y2);
        for (int y = y1; y <= y2; y++){
            setPixel(surface, x1, y, color);
        }
    }

    void drawLine(SDL_Surface* surface, float x1, float y1, float x2, float y2, const Uint16 color) {
        if (x1 == x2 || y1 == y2) {
            // Orthogonal line, use simplified logic
            drawLineFast(surface, x1, y1, x2, y2, color);
        } else {
            // Bresenham's line algorithm
            // Plagiarised from: https://www.rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C++
            const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
            if (steep) {
                std::swap(x1, y1);
                std::swap(x2, y2);
            }

            if (x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            const float dx = x2 - x1;
            const float dy = fabs(y2 - y1);

            float error = dx / 2.0f;
            const int ystep = (y1 < y2) ? 1 : -1;
            int y = (int)y1;

            const int maxX = (int)x2;

            for (int x = (int)x1; x <= maxX; x++) {
                if (steep) {
                    setPixel(surface, y, x, color);
                } else {
                    setPixel(surface, x, y, color);
                }

                error -= dy;
                if (error < 0) {
                    y += ystep;
                    error += dx;
                }
            }
        }
    }
} // namespace DrawUtils


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
