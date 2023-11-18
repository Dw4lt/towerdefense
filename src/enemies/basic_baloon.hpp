#pragma once
#include "enemy.hpp"


class BasicBaloon : public Enemy {
public:
    BasicBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType color);

    virtual ~BasicBaloon() = default;

    virtual void damage(int damage, DAMAGE_TYPE type) override;

protected:

    static int getColor(int hp);

    static int getSpeed(int hp);
};
