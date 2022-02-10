#ifndef ARCHER_H
#define ARCHER_H
#include "../enemies/enemy.hpp"
#include "../primitives/damage.hpp"
#include "tower.hpp"

class Archer : public Tower {
public:
    Archer(int cooldown, int tower_range, double damage, Tile* tile);
    virtual ~Archer();
    virtual void fire(std::vector<Enemy>& enemy_list);

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