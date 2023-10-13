#include "game_state.hpp"
#include "rendering/composable_scene.hpp"

ROwner<GameState> GameState::singleton_{nullptr};

GameState::GameState()
    : field_ptr_{new Field(0, 0, FIELD_WIDTH, FIELD_HEIGHT)}
    , wave_count_{0}
    , lives_{100}
{}

auto GameState::getField() -> Field& {
    return *field_ptr_;
}

auto GameState::getFieldReader() -> RReader<Field> {
    return field_ptr_;
}

auto GameState::getState() -> GameStatePtr {
    if (!GameState::singleton_) {
        GameState::singleton_ = ROwner(new GameState());
    }
    return GameState::singleton_;
}

auto GameState::addStructure(std::shared_ptr<Structure> structure, Tile& tile) -> RReader<Structure> {
    structure_list_.push_back(ROwner<Structure>(structure));
    tile.addChild(structure_list_.back().makeReader());
    tile.updateType(TileType::STRUCTURE);
    return structure_list_.back().makeReader();
}


auto GameState::addEnemy(std::shared_ptr<Enemy> enemy) -> RReader<Enemy> {
    enemy_list_.push_back(ROwner(enemy));
    return enemy_list_.back().makeReader();
}

void GameState::purgeEnemies(std::function<bool(Enemy&)> func) {
    for(auto e = enemy_list_.begin(); e != enemy_list_.end(); ) {
        auto& enemy = *e;
        if (func(*enemy)) {
            e = enemy_list_.erase(e);
        } else {
            e++;
        }
    }
}
