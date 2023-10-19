#include "game_state.hpp"
#include "rendering/composable_scene.hpp"

ROwner<GameState> GameState::singleton_{nullptr};

GameState::GameState()
    : structures_{}
    , enemy_list_{}
    , field_ptr_{new Field(0, 0, FIELD_WIDTH, FIELD_HEIGHT)}
    , wave_count_{11}
    , lives_{100}
    , money_{400}
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
    auto r = ROwner<Structure>(structure);

    tile.addChild(r.makeReader());
    tile.updateType(TileType::STRUCTURE);
    auto reader = r.makeReader();
    structures_[tile.getIndexY() * FIELD_TILE_COUNT_X + tile.getIndexX()] = std::move(r);
    return reader;
}

auto GameState::getStructure(int index_x, int index_y) -> RReader<Structure> {
    return structures_[index_y * FIELD_TILE_COUNT_X + index_x].makeReader();
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
