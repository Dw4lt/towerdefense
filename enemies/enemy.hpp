#ifndef ENEMY
#define ENEMY
#include <SDL/SDL.h>
#include "../rendering/renderer_object.hpp"

class Renderer;
class Field;

class Enemy : public RendererObject {
public:
    Enemy(Point pos, int width, int height, Point current_field, long int hp_, double speed, Uint16 color);
    virtual ~Enemy();
    virtual void render(Renderer* renderer) override;
    virtual void pathfind(Field* field);
    virtual Rect boundingBox() const override;

private:
    Point current_field_;
    double real_x_;
    double real_y_;
    Uint16 color_;
    long int hp_;
    double speed_;
};

#endif