#include "basic_baloon.hpp"
#include "../primitives/color_conversion.hpp"

BasicBaloon::BasicBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType color)
    : Enemy(pos, width, height, target_tile_index, (int) color, getSpeed((int) color), getColor((int) color)) {
}

void BasicBaloon::damage(int damage, DAMAGE_TYPE type) {
    hp_ -= damage;
    color_ = getColor(hp_);
    speed_ = getSpeed(hp_);
}

int BasicBaloon::getColor(int hp) {
    switch (hp) {
        case (int)EnemyType::RED:
            return RGB_888_TO_565(0xFF'03'03);
        case (int)EnemyType::BLUE:
            return RGB_888_TO_565(0x03'03'FF);
        case (int)EnemyType::GREEN:
            return RGB_888_TO_565(0x03'FF'03);
        case (int)EnemyType::YELLOW:
            return RGB_888_TO_565(0xE0'DA'19);
        case (int)EnemyType::PINK:
            return RGB_888_TO_565(0xFF'14'93);
        default:
            return RGB_888_TO_565(0xFF'8C'00); // Orange = Invalid
    };
}

int BasicBaloon::getSpeed(int hp) {
    switch (hp) {
        case (int)EnemyType::RED:
            return BASE_BALOON_SPEED * 1;
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
