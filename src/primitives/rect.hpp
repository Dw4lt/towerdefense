#pragma once
#include <SDL/SDL_video.h>
#include <SDL/SDL_stdinc.h>

#include "point.hpp"

template <typename T, typename U>
class RectTemplate {
public:
    RectTemplate(PointTemplate<T> origin, U width, U height)
        : origin_(origin), width_(width), height_(height) {};

    RectTemplate(T x = 0, T y = 0, U width = 1, U height = 1)
        : origin_{x, y}, width_{width}, height_{height} {};

    RectTemplate<T, U> centeredOn(const PointTemplate<T>& point) const {
        return RectTemplate<T, U>(point.x_ - width_ / 2, point.y_ - height_/2, width_, height_);
    };

    SDL_Rect toSDLRect() const {
        return SDL_Rect{(Sint16)origin_.x_, (Sint16)origin_.y_, (Uint16)width_, (Uint16)height_};
    };

    int right() const { return origin_.x_ + width_; };
    int top() const { return origin_.y_; };
    int left() const { return origin_.x_; };
    int bottom() const { return origin_.y_ + height_; };

    PointTemplate<T> center() const {
        return PointTemplate<T>(
            origin_.x_ + (T)std::floor(width_ / 2.0),
            origin_.y_ + (T)std::floor(height_ / 2.0)
        );
    };

    PointTemplate<T> origin_;
    U width_;
    U height_;

    bool contains(const PointTemplate<T>& point) const {
        return point.x_ >= origin_.x_ && point.y_ >= origin_.y_ && point.x_ < origin_.x_ + width_ && point.y_ < origin_.y_ + height_;
    };
};

using Rect = RectTemplate<Sint16, Uint16>;
