#include "game_state.hpp"

GameState* GameState::singleton_{nullptr};

GameState::GameState() {

}

GameState* GameState::Get() {
    if (GameState::singleton_ == nullptr) {
        GameState::singleton_ = new GameState();
    }
    return GameState::singleton_;
}
