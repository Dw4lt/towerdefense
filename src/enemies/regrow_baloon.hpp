#pragma once
#include "enemy.hpp"


class RegrowBaloon : public Enemy {
public:
    RegrowBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType color);

    virtual ~RegrowBaloon() = default;

    virtual void render(SDL_Surface* surface) override;

    virtual void tick(const Field& field) override;

    virtual void heal();

    virtual void damage(int damage, DAMAGE_TYPE type) override;

protected:

    static int getColor(EnemyType type);

    static int getSpeed(EnemyType type);

    static int getHP(EnemyType type);

    /// @brief Max hp the baloon can heal back to
    const int max_health;

    /// @brief Lowest hp the baloon was ever at.
    /// @brief Needed to prevent handing out money when repeatedly popping the same layer.
    int lowest_hp_so_far;

    unsigned int next_heal_countdown;

    static const unsigned int DELAY_BETWEEN_HEALS;
};
