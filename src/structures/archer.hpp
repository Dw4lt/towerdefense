#pragma once
#include "../enemies/enemy.hpp"
#include "tower.hpp"

class Archer : public Tower {
public:
    Archer(unsigned int index_x, unsigned int index_y);
    virtual ~Archer();

    virtual void render(SDL_Surface* surface) override;

    /// @brief Attempts to fire at an enemy within range
    /// @return Whether or not an enemy was found
    virtual bool fire() override;

    virtual TowerType getTowerType() const noexcept override { return TowerType::ARCHER; };

private:
    static double global_range_multiplier_;
    static double global_cooldown_multiplier_;
    static double global_damage_multiplier_;
};
