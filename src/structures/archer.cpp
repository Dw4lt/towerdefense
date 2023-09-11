#include "archer.hpp"
#include "../rendering/composable_scene.hpp"

Archer::Archer(int cooldown, int tower_range, double damage, const Tile& tile)
    : Tower(cooldown, tower_range, damage, tile)
    {
}

Archer::~Archer() {}

bool Archer::fire(RReaderIterable<Enemy> enemy_list) {
    for (auto enemy : enemy_list) {
        if (enemy.isValid() && withinRange(enemy.get())) {
            enemy->damage(damage_ * getGlobalDamageMultiplier(), DAMAGE_TYPE::PROJECTILE);
            return true;
        }
    }
    return false;
}
