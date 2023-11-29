#include "enemy_factory.hpp"
#include "basic_baloon.hpp"
#include "regrow_baloon.hpp"


// Values taken from: https://bloons.fandom.com/wiki/Rounds_(BTD5)
const std::vector<std::vector<EnemyCount>> EnemyFactory::waves {
    { {count: 20, type: EnemyType::RED} },
    { {count: 30, type: EnemyType::RED} },
    { {count: 20, type: EnemyType::RED}, {count: 5, type: EnemyType::BLUE} },
    { {count: 30, type: EnemyType::RED}, {count: 15, type: EnemyType::BLUE} },
    // 5
    { {count: 5, type: EnemyType::RED}, {count: 25, type: EnemyType::BLUE} },
    { {count: 15, type: EnemyType::RED}, {count: 15, type: EnemyType::BLUE}, {count: 4, type: EnemyType::GREEN} },
    { {count: 20, type: EnemyType::RED}, {count: 25, type: EnemyType::BLUE}, {count: 5, type: EnemyType::GREEN} },
    { {count: 10, type: EnemyType::RED}, {count: 20, type: EnemyType::BLUE}, {count: 14, type: EnemyType::GREEN} },
    { {count: 30, type: EnemyType::GREEN} },
    // 10
    { {count: 102, type: EnemyType::BLUE} },
    { {count: 10, type: EnemyType::RED}, {count: 10, type: EnemyType::BLUE}, {count: 12, type: EnemyType::GREEN}, {count: 2, type: EnemyType:: YELLOW} },
    { {count: 15, type: EnemyType::BLUE}, {count: 10, type: EnemyType::GREEN}, {count: 5, type: EnemyType:: YELLOW} },
    { {count: 100, type: EnemyType::RED}, {count: 23, type: EnemyType::GREEN}, {count: 4, type: EnemyType:: YELLOW} },
    { {count: 50, type: EnemyType::RED}, {count: 15, type: EnemyType::BLUE}, {count: 10, type: EnemyType::GREEN}, {count: 9, type: EnemyType:: YELLOW} },
    // 15
    { {count: 20, type: EnemyType::RED}, {count: 12, type: EnemyType::GREEN}, {count: 5, type: EnemyType::YELLOW}, {count: 3, type: EnemyType:: PINK} },
    { {count: 20, type: EnemyType::GREEN}, {count: 8, type: EnemyType::YELLOW}, {count: 4, type: EnemyType:: PINK} },
    { {count: 8, type: EnemyType::REGROW_YELLOW} },
    { {count: 80, type: EnemyType::GREEN} },
    { {count: 10, type: EnemyType::GREEN}, {count: 4, type: EnemyType::YELLOW}, {count: 5, type: EnemyType::REGROW_YELLOW}, {count: 7, type: EnemyType:: PINK} },
};

ROwner<Enemy> EnemyFactory::makeEnemy(EnemyType type, Point pos, unsigned int target_tile_index) {
    switch (type)
    {
    case EnemyType::RED:
    case EnemyType::BLUE:
    case EnemyType::GREEN:
    case EnemyType::YELLOW:
    case EnemyType::PINK:
        return ROwner(new BasicBaloon(pos, 5, 5, target_tile_index, type));
    case EnemyType::REGROW_RED:
    case EnemyType::REGROW_BLUE:
    case EnemyType::REGROW_GREEN:
    case EnemyType::REGROW_YELLOW:
    case EnemyType::REGROW_PINK:
        return ROwner(new RegrowBaloon(pos, 5, 5, target_tile_index, type));
    default:
        throw "Unmatched enemy type";
    }
}
