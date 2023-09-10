#include "essentials.hpp"

Point::Point(Sint16 x, Sint16 y)
    : x_(x)
    , y_(y) {}

Rect::Rect(Point origin, Uint16 width, Uint16 height)
    : origin_(origin)
    , width_(width)
    , height_(height) {}

Rect::Rect(Sint16 x, Sint16 y, Uint16 width, Uint16 height)
    : Rect(Point(x, y), width, height) {}

Rect Rect::centeredOn(const Point& point) const {
    return Rect(point.x_ - width_ / 2, point.y_ - height_/2, width_, height_);
}

bool Rect::contains(const Point& point) const {
    return point.x_ >= origin_.x_ && point.y_ >= origin_.y_ && point.x_ < origin_.x_ + width_ && point.y_ < origin_.y_ + height_;
}

Point Rect::center() const {
    return Point((int)std::round(origin_.x_ + width_ / 2.0),
                 (int)std::round(origin_.y_ + height_ / 2.0));
}

SDL_Rect Rect::toSDLRect() const {
    return SDL_Rect{(Sint16)origin_.x_, (Sint16)origin_.y_, (Uint16)width_, (Uint16)height_};
}
