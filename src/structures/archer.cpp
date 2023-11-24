#include "archer.hpp"
#include "../rendering/composable_scene.hpp"
#include "../util/constants.hpp"
#include "../util/color.hpp"

Archer::Archer(int cooldown, int tower_range, double damage, unsigned int index_x, unsigned int index_y)
    : Tower(cooldown, tower_range, damage, index_x, index_y)
    {
}

Archer::~Archer() {}

void Archer::render(SDL_Surface* surface) {
    if (just_fired_){
        DrawUtils::fillColor(surface, boundingBox(), Colors::ARCHER_FIRING);
        just_fired_ = false;
    } else {
        DrawUtils::fillColor(surface, boundingBox(), Colors::ARCHER_IDLE);
    }
    renderChildren(surface);
}

bool Archer::fire() {
    auto game_state = GameState::getState();
    for (int tile_index : sorted_path_tile_indexes_in_range_) {
        const auto& enemies = game_state->getEnemiesOnTile(tile_index);
        for (auto& enemy : enemies) {
            if (enemy.isValid() && enemy->getHP() > 0) {
                enemy->damage(damage_ * getGlobalDamageMultiplier(), DAMAGE_TYPE::PROJECTILE);
                return true;
            }
        }
    }
    return false;
}
