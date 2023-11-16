#pragma once
#include <SDL/SDL_stdinc.h>

template <typename T>
class PointTemplate {
public:
    PointTemplate(T x = 0, T y = 0) : x_(x), y_(y) {};

    bool operator==(const PointTemplate<T>& other) const {
        return x_ == other.x_ && y_ == other.y_;
    };

    T x_;
    T y_;
};

using Point = PointTemplate<Sint16>;
