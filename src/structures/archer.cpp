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

bool Archer::fire(IIterable<RReader<Enemy>> enemy_list) {
    for (auto& enemy = enemy_list.begin(); enemy != enemy_list.end(); ++enemy) {
        if (withinRange(**enemy)) {
            enemy->damage(damage_ * getGlobalDamageMultiplier(), DAMAGE_TYPE::PROJECTILE);
            return true;
        }
    }
    return false;
}
