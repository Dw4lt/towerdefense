#ifndef ESSENTIALS
#define ESSENTIALS
#include<SDL/SDL.h>
#include <iostream>

class Point
{
public:
    Point(int x = 0, int y = 0);
    int x_;
    int y_;
};

class Rect {
public:
    Rect(Point origin, int width, int height);
    Rect(int x, int y, int width, int height);
    SDL_Rect toSDLRect();
    int right(){return origin_.x_ + width_;};
    int top(){return origin_.y_;};
    int left(){return origin_.x_;};
    int bottom(){return origin_.y_ + height_;};
    Point origin_;
    int width_;
    int height_;
    bool contains(const Point& point);
};
#define ErrorStream(x)  std::cout << "## Exception in " << __func__ << "() ##\n" << x << "\n####################\n"


#endif