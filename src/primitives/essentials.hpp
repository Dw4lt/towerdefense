#ifndef ESSENTIALS_H
#define ESSENTIALS_H
#include <SDL/SDL.h>
#include <iostream>

class Point {
public:
    Point(int x = 0, int y = 0);
    int x_;
    int y_;
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
    Rect(Point origin, int width, int height);
    Rect(int x, int y, int width, int height);
    SDL_Rect toSDLRect();
    int right() { return origin_.x_ + width_; };
    int top() { return origin_.y_; };
    int left() { return origin_.x_; };
    int bottom() { return origin_.y_ + height_; };
    Point origin_;
    int width_;
    int height_;
    bool contains(const Point& point);
};
#define ErrorStream(x) std::cout << "## Exception in " << __func__ << "() ##\n" \
                                 << x << "\n####################\n"

#endif
