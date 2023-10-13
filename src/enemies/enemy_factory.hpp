#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include "enemy.hpp"
#include "basic_baloon.hpp"
#include "../primitives/color_conversion.hpp"

namespace EnemyFactory {
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
        { EnemyCount(10, EnemyType::RED), EnemyCount(10, EnemyType::BLUE), EnemyCount(12, EnemyType::GREEN), EnemyCount(2, EnemyType:: YELLOW) },
        { EnemyCount(15, EnemyType::BLUE), EnemyCount(10, EnemyType::GREEN), EnemyCount(5, EnemyType:: YELLOW) },
        { EnemyCount(100, EnemyType::RED), EnemyCount(23, EnemyType::GREEN), EnemyCount(4, EnemyType:: YELLOW) },
        { EnemyCount(50, EnemyType::RED), EnemyCount(15, EnemyType::BLUE), EnemyCount(10, EnemyType::GREEN), EnemyCount(9, EnemyType:: YELLOW) },
        // 15
        { EnemyCount(20, EnemyType::RED), EnemyCount(12, EnemyType::GREEN), EnemyCount(5, EnemyType::YELLOW), EnemyCount(3, EnemyType:: PINK) },
    };

    /// @brief Create an enemy of a given type
    /// @param type Type
    /// @param pos Position of center of enemy in pixel coordinates
    /// @param target_tile_index Next tile to walk towards
    /// @return shared_ptr to new enemy
    std::shared_ptr<Enemy> makeEnemy(EnemyType type, Point pos, unsigned int target_tile_index) {
        switch (type)
        {
        case EnemyType::RED:
        case EnemyType::BLUE:
        case EnemyType::GREEN:
        case EnemyType::YELLOW:
        case EnemyType::PINK:
            return std::make_shared<BasicBaloon>(pos, 4, 4, target_tile_index, type);
            /* code */
            break;

        default:
            throw "Unmatched enemy type";
        }
    }

};

#endif
