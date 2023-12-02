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

    RectTemplate& centerOn(const PointTemplate<T>& point) noexcept {
        return centerOn(point.x_, point.y_);
    };

    RectTemplate& centerOn(T x, T y) noexcept {
        origin_.x_ = x - width_ / 2;
        origin_.y_ = y - height_ / 2;
        return *this;
    };

    RectTemplate& resize(U delta_width, U delta_height) noexcept {
        width_ += delta_width;
        height_ += delta_height;
        return *this;
    };

    static RectTemplate<T, U> centeredOn(const PointTemplate<T>& point, U width, U height) {
        return centeredOn(point.x_, point.y_, width, height);
    };

    static RectTemplate<T, U> centeredOn(T x, T y, U width, U height) {
        return RectTemplate<T, U>(x - width / 2, y - height / 2, width, height);
    };

    SDL_Rect toSDLRect() const {
        return SDL_Rect{(Sint16)origin_.x_, (Sint16)origin_.y_, (Uint16)width_, (Uint16)height_};
    };

    auto right() const { return origin_.x_ + width_ - 1; }; // -1 because a rect of size 1 equates to 1 pixel, the origin pixel
    T top() const { return origin_.y_; };
    T left() const { return origin_.x_; };
    auto bottom() const { return origin_.y_ + height_ - 1; }; // -1 because a rect of size 1 equates to 1 pixel, the origin pixel

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
