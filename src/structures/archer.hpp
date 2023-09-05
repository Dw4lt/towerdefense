#ifndef ARCHER_H
#define ARCHER_H
#include "../enemies/enemy.hpp"
#include "../primitives/damage.hpp"
#include "../game_state.hpp"
#include "tower.hpp"

class Archer : public Tower {
public:
    Archer(int cooldown, int tower_range, double damage, Tile* tile);
    virtual ~Archer();

    /// @brief Attempts to fire at an enemy within range
    /// @param enemy_list Enemies to look through
    /// @return Whether or not an enemy was found
    virtual bool fire(EnemyList& enemy_list) override;

private:
    static double global_range_multiplier_;
    static double global_cooldown_multiplier_;
    static double global_damage_multiplier_;

    int cooldown_;
    double damage_;
    int range_;

    int cooldown_timer_;
};

#endif
