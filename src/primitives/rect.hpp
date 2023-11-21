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
        return centeredOn(point, width_, height_);
    };

    static RectTemplate<T, U> centeredOn(const PointTemplate<T>& point, int width, int height) {
        return centeredOn(point.x_, point.y_, width, height);
    };

    static RectTemplate<T, U> centeredOn(T x, T y, int width, int height) {
        return RectTemplate<T, U>(x - width / 2, y - height / 2, width, height);
    };

    SDL_Rect toSDLRect() const {
        return SDL_Rect{(Sint16)origin_.x_, (Sint16)origin_.y_, (Uint16)width_, (Uint16)height_};
    };

    int right() const { return origin_.x_ + width_ - 1; }; // -1 because a rect of size 1 equates to 1 pixel, the origin pixel
    int top() const { return origin_.y_; };
    int left() const { return origin_.x_; };
    int bottom() const { return origin_.y_ + height_ - 1; }; // -1 because a rect of size 1 equates to 1 pixel, the origin pixel

    PointTemplate<T> center() const {
        return PointTemplate<T>(
            origin_.x_ + (T)std::round((width_ - 1) / 2.0), // -1 because a rect of size 1 equates to 1 pixel, the origin pixel
            origin_.y_ + (T)std::round((height_ - 1) / 2.0) // -1 because a rect of size 1 equates to 1 pixel, the origin pixel
        );
    };

    /// @brief Origin of rectangle, top-left corner
    PointTemplate<T> origin_;
    U width_;
    U height_;

    bool contains(const PointTemplate<T>& point) const {
        return point.x_ >= origin_.x_ && point.y_ >= origin_.y_ && point.x_ < origin_.x_ + width_ && point.y_ < origin_.y_ + height_;
    };
};

using Rect = RectTemplate<Sint16, Uint16>;
