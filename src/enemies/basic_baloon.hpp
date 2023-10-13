#ifndef BASIC_BALOON_H
#define BASIC_BALOON_H
#include "enemy.hpp"


class BasicBaloon : public Enemy {
public:
    BasicBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType color);

    virtual ~BasicBaloon() = default;

    virtual void damage(int damage, DAMAGE_TYPE type) override;

    int getColor(int hp);

    int getSpeed(int hp);
};

#endif
