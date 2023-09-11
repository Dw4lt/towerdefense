#ifndef ENEMY_H
#define ENEMY_H
#include "../primitives/damage.hpp"
#include "../rendering/renderer_object.hpp"
#include "../primitives/essentials.hpp"
#include "../map/field.hpp"
#include <SDL/SDL_video.h>

class Enemy : public RendererObject {
public:
    Enemy(Point pos, int width, int height, Point current_field, long int hp_, double speed, Uint16 color);
    virtual ~Enemy();
    virtual void render(SDL_Surface* surface) override;
    virtual void pathfind(Field& field);
    virtual Rect boundingBox() const override;
    virtual bool isImmune(DAMAGE_TYPE type);
    virtual void damage(int damage, DAMAGE_TYPE type);

    virtual long int getHP() const;
    virtual SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::ENEMY; };

private:
    void setNextTarget(const Field& field);

    Point current_tile_;
    Point current_target_tile_;
    double real_x_;
    double real_y_;
    Uint16 color_;
    long int hp_;
    double speed_;
};

#endif
