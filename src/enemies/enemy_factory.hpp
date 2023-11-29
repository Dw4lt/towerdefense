#pragma once
#include "enemy.hpp"
#include "../util/ownership.hpp"

// TODO: Better name
struct EnemyCount {
    const unsigned int count;
    const EnemyType type;
};

struct EnemyFactory {
    /// @brief Pre-determined waves
    static const std::vector<std::vector<EnemyCount>> waves;

    /// @brief Create an enemy of a given type
    /// @param type Type
    /// @param pos Position of center of enemy in pixel coordinates
    /// @param target_tile_index Next tile to walk towards
    /// @return shared_ptr to new enemy
    static ROwner<Enemy> makeEnemy(EnemyType type, Point pos, unsigned int target_tile_index);
};
