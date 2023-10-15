#include "enemy.hpp"
#include "../rendering/composable_scene.hpp"
#include <assert.h>
#include <stdio.h>

Enemy::Enemy(Point pos, int width, int height, unsigned int target_tile_index, long int hp, double speed, Uint16 color)
    : RendererObject(Point(pos.x_ - width / 2, pos.y_ - height / 2), width, height, SCREEN_LAYER::ENEMY) // TODO: rect_really should not be a part of RendererObject
    , target_tile_index_(target_tile_index)
    , real_x_(pos.x_)
    , real_y_(pos.y_)
    , color_(color)
    , hp_(hp)
    , speed_(speed) {
}

Enemy::~Enemy() {
}

bool Enemy::isImmune(DAMAGE_TYPE type) {
    return false;
}

int Enemy::getHP() const {
    return hp_;
}

Rect Enemy::boundingBox() const {
    return rect_;
}

void Enemy::render(SDL_Surface* surface) {
    DrawUtils::fillColor(surface, boundingBox(), color_);
    RendererObject::renderChildren(surface);
}

void Enemy::updateBoundingBox() {
    // TODO: why is rect_ a part of render-object again?
    rect_.origin_.x_ = ((Sint16) real_x_) - rect_.width_ / 2;
    rect_.origin_.y_ = ((Sint16) real_y_) - rect_.height_ / 2;
}

bool Enemy::walkTowards(const Point& target, double& distance_to_travel) {
    auto real_delta_x = target.x_ - real_x_;
    auto real_delta_y = target.y_ - real_y_;
    double abs_distance = std::abs(real_delta_x) + std::abs(real_delta_y);

    // Short-circuit
    if (abs_distance <= distance_to_travel) {
        real_x_ = target.x_;
        real_y_ = target.y_;
        distance_to_travel -= abs_distance;
        return true;
    }

    // Partial distance walk necessary
    if (real_delta_x) { // x
        double step_size = std::min(std::abs(real_delta_x), distance_to_travel);
        double signed_step = step_size * sign(real_delta_x);
        distance_to_travel -= step_size;
        real_x_ += signed_step;
    }
    if (real_delta_y){ // y
        double step_size = std::min(std::abs(real_delta_y), distance_to_travel);
        double signed_step = step_size * sign(real_delta_y);
        distance_to_travel -= step_size;
        real_y_ += signed_step;
    }
    return false;
}

void Enemy::pathfind(const Field& field) {
    double distance_to_travel = speed_;

    // Walk towards screen
    if (real_x_ < 0) {
        auto delta = std::min(std::abs(real_x_), distance_to_travel);
        real_x_ += delta;
        distance_to_travel -= delta;
    }

    // Walk on screen
    auto target = field.getPathTileCenter(target_tile_index_);
    while (distance_to_travel > 0 && target.x_ < SHRT_MAX) {
        if (walkTowards(target, distance_to_travel)) {
            // Target tile reached
            target_tile_index_++;
            target = field.getPathTileCenter(target_tile_index_);
        }
    }

    // Walk off screen
    if (distance_to_travel > 0) {
        real_x_ += distance_to_travel;
        distance_to_travel = 0;
    }

    updateBoundingBox();
}
