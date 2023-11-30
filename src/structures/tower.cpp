#include "tower.hpp"
#include "../rendering/composable_scene.hpp"

double Tower::global_range_multiplier_(1);
double Tower::global_cooldown_multiplier_(1);
double Tower::global_damage_multiplier_(1);

Tower::Tower(int cooldown, int tower_range, double damage, unsigned int index_x, unsigned int index_y)
    : Structure(Rect::centeredOn((index_x + 0.5) * FIELD_TILE_WIDTH, (index_y + 0.5) * FIELD_TILE_HEIGHT, FIELD_TILE_WIDTH *0.8, FIELD_TILE_HEIGHT * 0.8))
    , index_x_(index_x)
    , index_y_(index_y)
    , cooldown_(cooldown)
    , damage_(damage)
    , range_(tower_range)
    , cooldown_timer_(0)
    , sorted_path_tile_indexes_in_range_()
    , just_fired_(false)
{
    setRange(tower_range);
}

Tower::~Tower() {
}

void Tower::setRange(int range) {
    range_ = range;
    sorted_path_tile_indexes_in_range_ = GameState::getState()->getField().getSortedPathTilesWithinRange(index_x_, index_y_, (int)(range_ * getGlobalRangeMultiplier()));
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

void Tower::onEndOfWave() {
    cooldown_timer_ = 0;
    just_fired_ = false;
}

void Tower::tick() {
    if (cooldown_timer_ <= 0){
        just_fired_ = fire();
        if (just_fired_) cooldown_timer_ += cooldown_ * global_cooldown_multiplier_;
    } else {
        just_fired_ = false;
        cooldown_timer_--;
    }
}
