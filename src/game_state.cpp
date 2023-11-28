#include "game_state.hpp"
#include "rendering/composable_scene.hpp"
#include "util/macros.hpp"

#include <algorithm>

ROwner<GameState> GameState::singleton_{nullptr};

GameState::GameState()
    : structures_{}
    , enemy_list_{}
    , field_ptr_{new Field(0, 0, FIELD_WIDTH, FIELD_HEIGHT)}
    , tile_enemy_mapping_(field_ptr_->getPathLength())
    , wave_count_{0}
    , lives_{100}
    , money_{650}
{}

void GameState::resetState() {
    ROwner<GameState>temp(new GameState());
    std::swap(singleton_, temp);
}

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
    LOG("Structure added at %i:%i\n", tile.getIndexX(), tile.getIndexY());
    return reader;
}

auto GameState::getStructure(int index_x, int index_y) -> RReader<Structure> {
    return structures_[index_y * FIELD_TILE_COUNT_X + index_x].makeReader();
}

auto GameState::addEnemy(std::shared_ptr<Enemy> enemy) -> RReader<Enemy> {
    enemy_list_.emplace_back(enemy);
    return enemy_list_.back().makeReader();
}

auto GameState::startNextWave() -> unsigned int {
    return ++wave_count_;
}

void GameState::endWave() {
    money_ += 99 + wave_count_;
}

auto GameState::tryTakeMoney(int amount) -> bool {
    bool enough_money = money_ > amount;
    if (enough_money) money_ -= amount;
    return enough_money;
}

void GameState::purgeEnemies(std::function<bool(Enemy&)> func) {
    for(auto e = enemy_list_.begin(); e != enemy_list_.end(); ) {
        auto& enemy = *e;
        if (func(*enemy)) {
            removeEnemyFromField(enemy);
            e = enemy_list_.erase(e);
        } else {
            e++;
        }
    }
}

void GameState::updateEnemyTile(const RReader<Enemy>& enemy, int previous_tile_index, int current_tile_index) {
    if (previous_tile_index >= 0){
        auto& vec = tile_enemy_mapping_[previous_tile_index];
        auto i = std::find(vec.begin(), vec.end(), enemy);
        if (i == vec.end()) throw "Enemy not in mapping.";
        vec.erase(i);
    }
    if (current_tile_index >= 0) {
        tile_enemy_mapping_[current_tile_index].push_back(enemy);
    }
};

void GameState::removeEnemyFromField(const RReader<Enemy>& enemy) {
    updateEnemyTile(enemy, enemy->getCurrentPathTileIndex(), -1);
}

