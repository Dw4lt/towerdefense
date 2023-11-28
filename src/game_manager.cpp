#include "game_manager.hpp"
#include "game_state.hpp"
#include "input.hpp"
#include "structures/archer.hpp"
#include "enemies/enemy_factory.hpp"
#include <SDL/SDL_video.h>
#include <SDL/SDL_timer.h>
#include <os.h>
#include <stdio.h>
#include <string>


GameManager::GameManager()
    : screen_(new Screen(320, 240))
{
    init();
}

void GameManager::init() {
    auto game_state = GameState::getState();

    field_cursor_ = ROwner(new FieldCursor(this));

    // Main field
    field_scene_ = ComposableScene::create(screen_.get(), SDL_Rect{x: 0, y: 0, w: FIELD_WIDTH, h: FIELD_HEIGHT}, true);
    field_scene_->addToScene(game_state->getFieldReader());
    field_scene_->addToScene(field_cursor_.makeReader());

    // Status bar
    status_bar_scene_ = StatusBar::create(screen_.get(), SDL_Rect{x: 0, y: FIELD_HEIGHT, w: FIELD_WIDTH, h: SCREEN_HEIGHT - FIELD_HEIGHT}, true);

    // Shop
    shop_scene_ = Shop::create(screen_.get(), SDL_Rect{x: 0, y: 0, w: FIELD_WIDTH, h: FIELD_HEIGHT}, false);
}

void GameManager::spawnWave() {
    auto game_state = GameState::getState();
    unsigned int wave = game_state->startNextWave();
    auto& field = game_state->getField();
    auto& starting_point = field.getStart();
    auto pos = field.getTile(starting_point).center();
    pos.x_ -= 20; // TODO: If enemy spawns *on* the first tile, he's not part of list of entities on said tile, leading to an attempt to remove it from an empty list during pathfinding. Possible solutions:
    // Get start coordinate from field, since it has all the knowledge necessary to yield a coordinate.
    // Simply subtract the width of a tile. (not very clean as it requires a left-hand start of the path)

    if (wave < EnemyFactory::waves.size()) {
        auto& w = EnemyFactory::waves.at(wave);
        for (auto& count : w) {
            for (unsigned int i = 0; i < count.count; i++) {
                auto enemy = game_state->addEnemy(EnemyFactory::makeEnemy(count.type, pos, 0));
                field_scene_->addToScene(enemy);
                pos.x_ -= 12;
            }
        }
    }
}

void GameManager::mainGameLoop() {
    auto game_state = GameState::getState();

    // Update enemies
    IIterable<RReader<Enemy>> enemies = game_state->getEnemies();
    Field& field = game_state->getField();
    for (auto& enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
        int path_index = enemy->getCurrentPathTileIndex();
        enemy->tick(field);
        int new_path_index = enemy->getCurrentPathTileIndex();
        if (path_index != new_path_index) {
            game_state->updateEnemyTile(*enemy, path_index, new_path_index);
        }
    }

    // Tick towers
    IIterable<RReader<Structure>> structures = game_state->getStructures();
    for (auto& structure = structures.begin(); structure != structures.end(); ++structure) {
        structure->tick();
    }

    // Purge enemies
    handleEnemiesReachingTarget();
    removeDeadEnemies();

    /*if (!game_state->anyEnemiesPresent()) {
        // TODO: Emit end of wave event
    }*/
}

void GameManager::endWave() {
    GameState::getState()->endWave();
}

void GameManager::mainRenderLoop() {
    field_scene_->render(screen_);
    status_bar_scene_->render(screen_);
    screen_->flip();
}


void GameManager::lostGameLoop() {
    // TODO: "Lost" menu screen here.
}

void GameManager::lostRenderLoop() {
    field_scene_->render(screen_);
    status_bar_scene_->render(screen_);
    // TODO: render "you lost" screen
    screen_->flip();
}


void GameManager::shopRenderLoop() {
    shop_scene_->render(screen_);
    status_bar_scene_->render(screen_);
    screen_->flip();
}

void GameManager::reset() {
    GameState::resetState();
    init();
}

void GameManager::handleEnemiesReachingTarget() {
    const auto& state = GameState::getState();

    state->purgeEnemies(
        [&state] (Enemy& e){
            bool target_reached = e.center().x_ > FIELD_WIDTH + 20; // Magic grace number
            if (target_reached) {
                state->takeLives(e.getHP());
            }
            return target_reached;
        }
    );
}

void GameManager::removeDeadEnemies() {
    GameState::getState()->purgeEnemies(
        [](Enemy& e){return e.getHP() <= 0;}
    );
}

void GameManager::applyUserInputToFieldCursor(UserInputEvent& event) {
    field_cursor_->applyUserActions(event.getAutorepeat());
}

void GameManager::onMapCursorClickOn(int x, int y) {
    auto game_state = GameState::getState();
    auto& tile = game_state->getField().getTile(x, y);
    TileType type = tile.getType();
    if (TileType::LAND == type) {
        if (game_state->tryTakeMoney(170)) { // TODO: price determined based on tower
            auto structure = game_state->addStructure(std::make_shared<Archer>(24, 4, 1, x, y), tile); // TODO: pick class based on store selection
            field_scene_->addToScene(structure);
        }
    } else if (TileType::PATH == type) {
    }
}
