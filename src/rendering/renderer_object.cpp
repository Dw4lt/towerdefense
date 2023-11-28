#include "renderer_object.hpp"

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
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        // Note on +1: ensures rect is at least 1 pixel and both x2 and y2 are colored
        float w = x2 - x1 + 1;
        float h = y2 - y1 + 1;
        auto rect = SDL_Rect{x: (Sint16) x1, y: (Sint16)y1, w: (Uint16) w, h: (Uint16)h};
        SDL_FillRect(surface, &rect, color);
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

    void drawCircleSegments(SDL_Surface* surface, int xc, int yc, float start_x, float start_y, float end_x, float end_y, float x_scale, float y_scale, Uint16 color) {
        float sxx = std::round(start_x * x_scale);
        float syy = std::round(start_y * y_scale);
        float exx = std::round(end_x * x_scale);
        float eyy = std::round(end_y * y_scale);
        float syx = std::round(start_y * x_scale);
        float sxy = std::round(start_x * y_scale);
        float eyx = std::round(end_y * x_scale);
        float exy = std::round(end_x * y_scale);

        drawLine(surface, xc + sxx - 1, yc + syy - 1, xc + exx - 1, yc + eyy - 1, color);
        drawLine(surface, xc - sxx, yc + syy - 1, xc - exx, yc + eyy - 1, color);
        drawLine(surface, xc + sxx - 1, yc - syy + 1, xc + exx - 1, yc - eyy + 1, color);
        drawLine(surface, xc - sxx, yc - syy + 1, xc - exx, yc - eyy + 1, color);
        drawLine(surface, xc + syx - 1, yc + sxy - 1, xc + eyx - 1, yc + exy - 1, color);
        drawLine(surface, xc - syx, yc + sxy - 1, xc - eyx, yc + exy - 1, color);
        drawLine(surface, xc + syx - 1, yc - sxy + 1, xc + eyx - 1, yc - exy + 1, color);
        drawLine(surface, xc - syx, yc - sxy + 1, xc - eyx, yc - exy + 1, color);
    }

    void drawPixelatedCircleOutline(SDL_Surface* surface, int x_center, int y_center, float radius, float x_scale, float y_scale, Uint16 color, Uint8 thickness) {
        radius += 0.5;
        float x = -0.5;
        float y = radius;

        float old_x = 0;
        float old_y = y;

        // Midpoint circle algorithm
        float d = 1.25 - radius;
        do {
            x += 1;
            if (x <= y) drawCircleSegments(surface, x_center, y_center, old_x, y, x, y, x_scale, y_scale, color);
            if (d < 0) {
                d += 2 * x + 1;
            } else {
                y -= 1;
                d += 2 * (x - y) + 1;
                if (x <= y) {
                    drawCircleSegments(surface, x_center, y_center, x, y, x, old_y, x_scale, y_scale, color);
                }
            }

            old_x = x;
            old_y = y;
        } while (x <= y);
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

void RendererObject::centerOn(int x, int y) {
    rect_.origin_.x_ = x - rect_.width_ / 2;
    rect_.origin_.y_ = y - rect_.height_ / 2;
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

void RendererObject::setWidth(Uint16 width) {
    rect_.width_ = width;
}

void RendererObject::setHeight(Uint16 height) {
    rect_.height_ = height;
}

Point RendererObject::center() const {
    return rect_.center();
}
