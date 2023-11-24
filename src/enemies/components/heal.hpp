#pragma once

class Enemy;

/// @brief Periodically heals enemy after a certain delay
struct HealComponent{
    /// @param max_health Maximum hp the enemy can heal back to
    /// @param current_health Starting health
    /// @param ticks_between_heal Nr. of calls to tick() before regeneration applies again
    HealComponent(int max_health, int current_health, int ticks_between_heal);

    /// @brief Call this within the Enemy's tick() method, may modify the enemy's HP
    /// @param enemy Enemy to process
    void tick(Enemy* enemy);

    /// @brief Update internal values after damage and initialize heal timer if necessary
    /// @param enemy Enemy to process
    /// @return Amount newly popped layers, not counting regenerated layers
    int on_damage(const Enemy* enemy);

private:
    /// @brief Max hp the baloon can heal back to
    const int max_health_;

    /// @brief Lowest hp the baloon was ever at.
    /// @brief Needed to prevent handing out money when repeatedly popping the same layer.
    int lowest_hp_so_far_;

    unsigned int next_heal_countdown_;

    const int ticks_between_heal_;
};
