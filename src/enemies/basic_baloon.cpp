#include "basic_baloon.hpp"
#include "../primitives/color.hpp"
#include "../game_state.hpp"

BasicBaloon::BasicBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType color)
    : Enemy(pos, width, height, target_tile_index, (int) color, getSpeed((int) color), getColor((int) color)) {
}

void BasicBaloon::damage(int damage, DAMAGE_TYPE) {
    damage = std::min(damage, hp_);
    hp_ -= damage;
    GameState::getState()->addMoney(damage);
    color_ = getColor(hp_);
    speed_ = getSpeed(hp_);
}

int BasicBaloon::getColor(int hp) {
    switch (hp) {
        case (int)EnemyType::RED:
            return Colors::BALOON_RED;
        case (int)EnemyType::BLUE:
            return Colors::BALOON_BLUE;
        case (int)EnemyType::GREEN:
            return Colors::BALOON_GREEN;
        case (int)EnemyType::YELLOW:
            return Colors::BALOON_YELLOW;
        case (int)EnemyType::PINK:
            return Colors::BALOON_PINK;
        default:
            return Colors::UNKNOWN;
    };
}

int BasicBaloon::getSpeed(int hp) {
    switch (hp) {
        case (int)EnemyType::RED:
            return 1.0 * BASE_BALOON_SPEED;
        case (int)EnemyType::BLUE:
            return 1.4 * BASE_BALOON_SPEED;
        case (int)EnemyType::GREEN:
            return 1.8 * BASE_BALOON_SPEED;
        case (int)EnemyType::YELLOW:
            return 3.2 * BASE_BALOON_SPEED;
        case (int)EnemyType::PINK:
            return 3.5 * BASE_BALOON_SPEED;
        default:
            return 1;
    };
}
