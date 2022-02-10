#include "archer.hpp"
#include "../rendering/renderer.hpp"

Archer::Archer(int cooldown, int tower_range, double damage, Tile* tile)
    : Tower(cooldown, tower_range, damage, tile) {
}

Archer::~Archer() {
}

void Archer::fire(std::vector<Enemy>& enemy_list) {
    for (auto enemy : enemy_list) {
        if (withinRange(enemy)) {
            enemy.damage(damage_ * getGlobalDamageMultiplier(), DAMAGE_TYPE::PROJECTILE);
            return;
        }
    }
}