#ifndef ARCHER_H
#define ARCHER_H
#include "../enemies/enemy.hpp"
#include "../primitives/damage.hpp"
#include "../primitives/ownership.hpp"
#include "../game_state.hpp"
#include "tower.hpp"

class Archer : public Tower {
public:
    Archer(int cooldown, int tower_range, double damage, const Tile& tile);
    virtual ~Archer();

    virtual void render(SDL_Surface* surface) override;

    /// @brief Attempts to fire at an enemy within range
    /// @param enemy_list Enemies to look through
    /// @return Whether or not an enemy was found
    virtual bool fire(IIterable<RReader<Enemy>> enemy_list) override;

private:
    static double global_range_multiplier_;
    static double global_cooldown_multiplier_;
    static double global_damage_multiplier_;
};

#endif
