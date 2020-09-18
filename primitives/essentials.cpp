#include"essentials.hpp"

Point::Point(int x, int y)
    : x_(x)
    , y_(y)
{

}

Rect::Rect(Point origin, int width, int height)
    : origin_(origin)
    , width_(width)
    , height_(height)
{}

Rect::Rect(int x, int y, int width, int height)
    : Rect(Point(x,y), width, height)
{}

bool Rect::contains(const Point& point){
    return point.x_ >= origin_.x_ && point.y_ >= origin_.y_ && point.x_ < origin_.x_ + width_ && point.y_ < origin_.y_ + height_;
}

SDL_Rect Rect::toSDLRect(){
    return SDL_Rect{(Sint16)origin_.x_, (Sint16)origin_.y_, (Uint16)width_, (Uint16)height_};
}