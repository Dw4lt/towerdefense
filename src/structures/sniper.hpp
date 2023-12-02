#pragma once
#include "../enemies/enemy.hpp"
#include "../util/ownership.hpp"
#include "tower.hpp"

class Sniper : public Tower {
public:
    Sniper(unsigned int index_x = 0, unsigned int index_y = 0);
    virtual ~Sniper();

    virtual void render(SDL_Surface* surface) override;

    /// @brief Attempts to fire at an enemy within range
    /// @return Whether or not an enemy was found
    virtual bool fire() override;

    virtual TowerType getTowerType() const noexcept override { return TowerType::SNIPER; };

private:
    static double global_cooldown_multiplier_;
    static double global_damage_multiplier_;
};
