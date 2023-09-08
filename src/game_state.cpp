#include "game_state.hpp"
#include "rendering/renderer.hpp"

ROwner<GameState> GameState::singleton_{nullptr};

GameState::GameState()
    : field_ptr_{new Field(0, 0, FIELD_WIDTH, FIELD_HEIGHT)}
    {
    Renderer::get()->addToScene(field_ptr_.makeReader());
}

auto GameState::getField() -> Field& {
    return *field_ptr_;
}

auto GameState::getState() -> GameStatePtr {
    if (!GameState::singleton_) {
        GameState::singleton_ = ROwner(new GameState());
    }
    return GameState::singleton_;
}

void GameState::addStructure(std::shared_ptr<Structure> structure, Tile& tile){
    structure_list_.push_back(ROwner<Structure>(structure));
    tile.addChild(structure_list_.back().makeReader());
    tile.updateType(TileType::STRUCTURE);
}


void GameState::addEnemy(std::shared_ptr<Enemy> enemy){
    enemy_list_.push_back(ROwner(enemy));
    Renderer::get()->addToScene(enemy_list_.back().makeReader());
}

void GameState::purgeEnemies(bool (*func)(Enemy& e)) {
    for(auto e = enemy_list_.begin(); e != enemy_list_.end(); ) {
        auto& enemy = *e;
        if (func(*enemy)) {
            e = enemy_list_.erase(e);
        } else {
            e++;
        }
    }
}
