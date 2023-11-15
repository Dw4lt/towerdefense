#include "archer.hpp"
#include "../rendering/composable_scene.hpp"

Archer::Archer(int cooldown, int tower_range, double damage, const Tile& tile)
    : Tower(cooldown, tower_range, damage, tile)
    {
}

Archer::~Archer() {}

void Archer::render(SDL_Surface* surface) {
    if (just_fired_){
        DrawUtils::fillColor(surface, boundingBox(), RGB_888_TO_565(0x5197FF));
        just_fired_ = false;
    } else {
        DrawUtils::fillColor(surface, boundingBox(), RGB_888_TO_565(0x137BF0));
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
