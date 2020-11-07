#ifndef TOWER_H
#define TOWER_H
#include "structure.hpp"
#include "../enemies/enemy.hpp"

class Tower : public Structure {
public:
    Tower(int cooldown, int tower_range, double damage, Tile* tile);
    virtual ~Tower();

    virtual void render(Renderer* renderer);

    static double getGlobalRangeMultiplier();
    static void setGlobalRangeMultiplier(double new_multiplier);
    static double getGlobalCooldownMultiplier();
    static void setGlobalCooldownMultiplier(double new_multiplier);
    static double getGlobalDamageMultiplier();
    static void setGlobalDamageMultiplier(double new_multiplier);

    virtual void tick(int elapsed_time_in_ms, std::vector<Enemy>& enemy_lis);
    bool withinRange(const Enemy& enemy) const;

    virtual void fire(std::vector<Enemy>& enemy_list) = 0;

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