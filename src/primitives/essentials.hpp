#ifndef ESSENTIALS_H
#define ESSENTIALS_H
#include <SDL/SDL_video.h>
#include <iostream>

class Point {
public:
    Point(Sint16 x = 0, Sint16 y = 0);
    bool operator==(const Point& other) const;

    Sint16 x_;
    Sint16 y_;
};

enum SCREEN_LAYER {
    BACKGROUND = 0,
    RESOURCE,
    STRUCTURE,
    ENEMY,
    CURSOR,

    SHOP,
};

static const int STANDARD_TICK_DURATION(41);

static const int FIELD_HEIGHT = 210;
static const int FIELD_WIDTH = 320;
static const int FIELD_TILE_COUNT_X = 40;
static const int FIELD_TILE_COUNT_Y = 30;

class Rect {
public:
    Rect(Point origin, Uint16 width, Uint16 height);
    Rect(Sint16 x = 0, Sint16 y = 0, Uint16 width = 1, Uint16 height = 1);

    Rect centeredOn(const Point& point) const;

    SDL_Rect toSDLRect() const;
    int right() const { return origin_.x_ + width_; };
    int top() const { return origin_.y_; };
    int left() const { return origin_.x_; };
    int bottom() const { return origin_.y_ + height_; };
    Point center() const;

    Point origin_;
    Uint16 width_;
    Uint16 height_;

    bool contains(const Point& point) const;
};

#define sign(x) ((x) > 0) - ((x) < 0)

#define ErrorStream(x) std::cout << "## Exception in " << __func__ << "() ##\n" \
                                 << x << "\n####################\n"

#endif
