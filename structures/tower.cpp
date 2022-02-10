#include "tower.hpp"
#include "../rendering/renderer.hpp"

double Tower::global_range_multiplier_(1);
double Tower::global_cooldown_multiplier_(1);
double Tower::global_damage_multiplier_(1);

Tower::Tower(int cooldown, int tower_range, double damage, Tile* tile)
    : Structure(tile)
    , cooldown_(cooldown)
    , damage_(damage)
    , range_(tower_range)
    , cooldown_timer_(0) {
}

Tower::~Tower() {
}

bool Tower::withinRange(const Enemy& enemy) const {
    if (range_ < 0) {
        return true;
    }
    auto epos = enemy.getCenter();
    auto pos = getCenter();
    auto delta_x = epos.x_ - pos.x_;
    auto delta_y = epos.y_ - pos.y_;
    auto tot_range = range_ * getGlobalRangeMultiplier();
    return delta_x * delta_x + delta_y * delta_y <= tot_range * tot_range;
}

double Tower::getGlobalRangeMultiplier() {
    return global_range_multiplier_;
}

void Tower::setGlobalRangeMultiplier(double new_multiplier) {
    global_range_multiplier_ = new_multiplier;
}

double Tower::getGlobalCooldownMultiplier() {
    return global_cooldown_multiplier_;
}

void Tower::setGlobalCooldownMultiplier(double new_multiplier) {
    global_cooldown_multiplier_ = new_multiplier;
}

double Tower::getGlobalDamageMultiplier() {
    return global_damage_multiplier_;
}

void Tower::setGlobalDamageMultiplier(double new_multiplier) {
    global_damage_multiplier_ = new_multiplier;
}

void Tower::render(Renderer* renderer) {
    renderer->fillColor(boundingBox(), RGB_888_TO_565(0xFF00FF));
    RendererObject::renderChildren(renderer);
}

void Tower::tick(int elapsed_time_in_ms, std::vector<Enemy>& enemy_list) {
    cooldown_timer_ -= elapsed_time_in_ms;
    while (cooldown_timer_ <= 0) {
        fire(enemy_list);
        cooldown_timer_ += cooldown_ * global_cooldown_multiplier_;
    }
}