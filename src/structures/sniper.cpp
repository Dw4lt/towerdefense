#include "sniper.hpp"
#include "../rendering/composable_scene.hpp"
#include "../util/color.hpp"

Sniper::Sniper(unsigned int index_x, unsigned int index_y)
    : Tower(SNIPER_BASE_RELOAD, SNIPER_BASE_RANGE, SNIPER_BASE_DAMAGE, index_x, index_y)
{
}

Sniper::~Sniper() {}

void Sniper::render(SDL_Surface* surface) {
    if (just_fired_){
        DrawUtils::fillColor(surface, boundingBox(), Colors::SNIPER_FIRING);
    } else {
        DrawUtils::fillColor(surface, boundingBox(), Colors::SNIPER_IDLE);
    }
    renderChildren(surface);
}

bool Sniper::fire() {
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
