#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include "enemy.hpp"
#include "../primitives/color_conversion.hpp"

namespace EnemyFactory {
    enum class EnemyType {
        RED,
        BLUE,
        GREEN,
    };

    // TODO: Better name
    struct EnemyCount {
        EnemyCount(unsigned int count_, EnemyType type_) : count{count_}, type{type_} {};
        const unsigned int count;
        const EnemyType type;
    };

    typedef std::vector<EnemyCount> Wave;

    // Pre-determined waves
    // Values taken from: https://bloons.fandom.com/wiki/Rounds_(BTD5)
    std::vector<Wave> waves {
        { EnemyCount(20, EnemyType::RED) },
        { EnemyCount(30, EnemyType::RED) },
        { EnemyCount(20, EnemyType::RED), EnemyCount(5, EnemyType::BLUE) },
        { EnemyCount(30, EnemyType::RED), EnemyCount(15, EnemyType::BLUE) },
        // 5
        { EnemyCount(5, EnemyType::RED), EnemyCount(25, EnemyType::BLUE) },
        { EnemyCount(15, EnemyType::RED), EnemyCount(15, EnemyType::BLUE), EnemyCount(4, EnemyType::GREEN) },
        { EnemyCount(20, EnemyType::RED), EnemyCount(25, EnemyType::BLUE), EnemyCount(5, EnemyType::GREEN) },
        { EnemyCount(10, EnemyType::RED), EnemyCount(20, EnemyType::BLUE), EnemyCount(14, EnemyType::GREEN) },
        { EnemyCount(30, EnemyType::GREEN) },
        // 10
        { EnemyCount(102, EnemyType::BLUE) },
    };

    const float BASE_SPEED = 1.5;

    /// @brief Create an enemy of a given type
    /// @param type Type
    /// @param pos Position of center of enemy in pixel coordinates
    /// @param target_tile_index Next tile to walk towards
    /// @return shared_ptr to new enemy
    std::shared_ptr<Enemy> makeEnemy(EnemyType type, Point pos, unsigned int target_tile_index) {
        switch (type)
        {
        case EnemyType::RED:
            return std::make_shared<Enemy>(pos, 4, 4, target_tile_index, 1, 1 * BASE_SPEED, RGB_888_TO_565(0xFF'03'03));
        case EnemyType::BLUE:
            return std::make_shared<Enemy>(pos, 4, 4, target_tile_index, 2, 1.4 * BASE_SPEED, RGB_888_TO_565(0x03'03'FF));
        case EnemyType::GREEN:
            return std::make_shared<Enemy>(pos, 4, 4, target_tile_index, 2, 1.8 * BASE_SPEED, RGB_888_TO_565(0x03'FF'03));
        default:
            throw "Unmatched enemy type";
        }
    }

};

#endif
