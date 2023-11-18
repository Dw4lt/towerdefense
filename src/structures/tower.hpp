#pragma once
#include "../enemies/enemy.hpp"
#include "../game_state.hpp"
#include "structure.hpp"

#include <vector>

class Tower : public Structure {
public:
    Tower(int cooldown, int tower_range, double damage, unsigned int index_x, unsigned int index_y);
    virtual ~Tower();

    virtual void render(SDL_Surface* surface) = 0;

    static double getGlobalRangeMultiplier();
    static void setGlobalRangeMultiplier(double new_multiplier);
    static double getGlobalCooldownMultiplier();
    static void setGlobalCooldownMultiplier(double new_multiplier);
    static double getGlobalDamageMultiplier();
    static void setGlobalDamageMultiplier(double new_multiplier);

    virtual void tick() override;

    /// @brief Attempts to fire at an enemy within range.
    /// @note Enemies should be searched for via the tiles within range and the tile-to-enemies mapping.
    /// @return Whether or not an enemy was found
    virtual bool fire() = 0;

    /// @brief Get range of tower in tiles
    int getRange() const { return range_; };

    /// @brief Change the range of the tower. Requires fied to be fully initialized
    void setRange(int range);

    /// @brief Reset tower's cooldown, making it ready to fire instantly
    void resetCooldown();

private:
    static double global_range_multiplier_;
    static double global_cooldown_multiplier_;
    static double global_damage_multiplier_;

protected:
    unsigned int index_x_;
    unsigned int index_y_;

    int cooldown_;
    double damage_;
    int range_;

    int cooldown_timer_;

    /// @brief Path tiles within turret range, along with index on path. Furthest tile appears first.
    std::vector<int> sorted_path_tile_indexes_in_range_;

    /// @brief Flag set when a tower has succesfully fired. Resets upon render.
    bool just_fired_;
};
