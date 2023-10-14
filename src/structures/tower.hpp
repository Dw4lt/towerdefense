#ifndef TOWER_H
#define TOWER_H
#include "../enemies/enemy.hpp"
#include "../game_state.hpp"
#include "structure.hpp"

class Tower : public Structure {
public:
    Tower(int cooldown, int tower_range, double damage, const Tile& tile);
    virtual ~Tower();

    virtual void render(SDL_Surface* surface) = 0;

    static double getGlobalRangeMultiplier();
    static void setGlobalRangeMultiplier(double new_multiplier);
    static double getGlobalCooldownMultiplier();
    static void setGlobalCooldownMultiplier(double new_multiplier);
    static double getGlobalDamageMultiplier();
    static void setGlobalDamageMultiplier(double new_multiplier);

    virtual void tick() override;

    bool withinRange(const Enemy* enemy) const;

    /// @brief Attempts to fire at an enemy within range
    /// @param enemy_list Enemies to look through
    /// @return Whether or not an enemy was found
    virtual bool fire(RReaderIterable<Enemy> enemy_list) = 0;

private:
    static double global_range_multiplier_;
    static double global_cooldown_multiplier_;
    static double global_damage_multiplier_;

protected:
    int cooldown_;
    double damage_;
    int range_;

    int cooldown_timer_;

    /// @brief Flag set when a tower has succesfully fired. Resets upon render.
    bool just_fired_;
};

#endif
