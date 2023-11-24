#include "heal.hpp"
#include "../../game_state.hpp"

HealComponent::HealComponent(int max_health, int current_health, int ticks_between_heal)
    : max_health_{max_health}
    , lowest_hp_so_far_{current_health}
    , next_heal_countdown_{0}
    , ticks_between_heal_{ticks_between_heal} {
}

void HealComponent::tick(Enemy* enemy) {
    int hp = enemy->getHP();
    if (hp < max_health_) {
        if (next_heal_countdown_ > 0) {
            next_heal_countdown_--;
        } else {
            int delta = max_health_ - hp;
            if (delta > 0) enemy->heal(1);
            if (delta > 1) next_heal_countdown_ = ticks_between_heal_;
        }
    }
}

int HealComponent::on_damage(const Enemy* enemy) {
    int hp = enemy->getHP();
    auto layers_popped_first_time = lowest_hp_so_far_ - hp;
    lowest_hp_so_far_ = std::min(lowest_hp_so_far_, hp);

    if (next_heal_countdown_ == 0) next_heal_countdown_ = ticks_between_heal_;

    return std::max(layers_popped_first_time, 0);
}
