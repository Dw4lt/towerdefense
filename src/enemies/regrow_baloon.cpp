#include "regrow_baloon.hpp"
#include "../util/color.hpp"
#include "../game_state.hpp"

const unsigned int RegrowBaloon::DELAY_BETWEEN_HEALS{TICKS_PER_SECOND * 3};

RegrowBaloon::RegrowBaloon(Point pos, int width, int height, unsigned int target_tile_index, EnemyType type)
    : Enemy(pos, width, height, target_tile_index, getHP(type), getSpeed(type), getColor(type))
    , max_health {hp_}
    , lowest_hp_so_far{hp_}
    , next_heal_countdown{0}
{
}

void RegrowBaloon::render(SDL_Surface* surface) {
    auto bounding_box = boundingBox();
    auto bb_center = bounding_box.center();

    DrawUtils::fillColor(surface, bounding_box, color_);

    // "+" symbol
    DrawUtils::drawLine(surface, bb_center.x_, bounding_box.top() + 1, bb_center.x_, bounding_box.bottom() - 1, 0x0000);
    DrawUtils::drawLine(surface, bounding_box.left() + 1, bb_center.y_, bounding_box.right() - 1, bb_center.y_, 0x0000);
}

void RegrowBaloon::damage(int damage, DAMAGE_TYPE) {
    damage = std::min(damage, hp_);
    hp_ -= damage;

    // Give popping reward, without heal-farming exploit
    auto layers_popped_first_time = lowest_hp_so_far - hp_;
    lowest_hp_so_far = std::min(lowest_hp_so_far, hp_);
    if (layers_popped_first_time > 0) GameState::getState()->addMoney(layers_popped_first_time);

    if (hp_ > 0) {
        auto type = static_cast<EnemyType>(hp_ + (int)EnemyType::REGROW_RED - 1);
        color_ = getColor(type);
        speed_ = getSpeed(type);
        if (next_heal_countdown == 0) next_heal_countdown = DELAY_BETWEEN_HEALS;
    }
}

void RegrowBaloon::tick(const Field& field) { // TODO: Strategy pattern or similar. This behaviour will appear multiple times.
    // Regen health if necessary
    if (hp_ < max_health) {
        if (next_heal_countdown > 0) {
            next_heal_countdown--;
        } else {
            heal();
            if (hp_ < max_health) next_heal_countdown = DELAY_BETWEEN_HEALS;
        }
    }

    pathfind(field);
}

void RegrowBaloon::heal() {
    hp_ = std::min(hp_ + 1, max_health);
    auto type = static_cast<EnemyType>(hp_ + (int)EnemyType::REGROW_RED - 1);
    color_ = getColor(type);
    speed_ = getSpeed(type);
}

int RegrowBaloon::getHP(EnemyType type) {
    return (int) type - (int) EnemyType::REGROW_RED + 1;
}

int RegrowBaloon::getColor(EnemyType type) {
    switch (type) {
        case EnemyType::REGROW_RED:
            return Colors::BALOON_RED;
        case EnemyType::REGROW_BLUE:
            return Colors::BALOON_BLUE;
        case EnemyType::REGROW_GREEN:
            return Colors::BALOON_GREEN;
        case EnemyType::REGROW_YELLOW:
            return Colors::BALOON_YELLOW;
        case EnemyType::REGROW_PINK:
            return Colors::BALOON_PINK;
        default:
            return Colors::UNKNOWN;
    };
}

int RegrowBaloon::getSpeed(EnemyType type) {
    switch (type) {
        case EnemyType::REGROW_RED:
            return 1.0 * BASE_BALOON_SPEED;
        case EnemyType::REGROW_BLUE:
            return 1.4 * BASE_BALOON_SPEED;
        case EnemyType::REGROW_GREEN:
            return 1.8 * BASE_BALOON_SPEED;
        case EnemyType::REGROW_YELLOW:
            return 3.2 * BASE_BALOON_SPEED;
        case EnemyType::REGROW_PINK:
            return 3.5 * BASE_BALOON_SPEED;
        default:
            return 1;
    };
}
