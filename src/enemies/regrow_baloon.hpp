#pragma once
#include "enemy.hpp"
#include "components/heal.hpp"


class RegrowBaloon : public Enemy {
public:
    RegrowBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType color);

    virtual ~RegrowBaloon() = default;

    virtual void render(SDL_Surface* surface) override;

    virtual void tick(const Field& field) override;

    virtual void heal(int heal) override;

    virtual void damage(int damage, DAMAGE_TYPE type) override;

protected:

    static int getColor(EnemyType type);

    static int getSpeed(EnemyType type);

    static int getHP(EnemyType type);

    /// @brief Component to handle regen effect
    HealComponent heal_component_;

    static const unsigned int DELAY_BETWEEN_HEALS;
};
