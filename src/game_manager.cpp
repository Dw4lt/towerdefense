#include "game_manager.hpp"
#include "primitives/input.hpp"
#include "structures/archer.hpp"
#include "enemies/enemy_factory.hpp"
#include <SDL/SDL_video.h>
#include <SDL/SDL_timer.h>
// #include <ctime>
#include <iostream>
#include <os.h>
#include <stdio.h>
#include <string>

GameManager::GameManager()
    : screen_(new Screen(320, 240))
{
    auto game_state = GameState::getState();

    field_cursor_ = ROwner(new FieldCursor(this));

    // Main field
    field_scene_ = ComposableScene::create(screen_.get(), SDL_Rect{x: 0, y: 0, w: FIELD_WIDTH, h: FIELD_HEIGHT}, true);
    field_scene_->addToScene(game_state->getFieldReader());
    field_scene_->addToScene(field_cursor_.makeReader());

    // Status bar
    status_bar_scene_ = StatusBar::create(screen_.get(), SDL_Rect{x: 0, y: FIELD_HEIGHT, w: FIELD_WIDTH, h: SCREEN_HEIGHT - FIELD_HEIGHT}, true);
}

void GameManager::spawnWave() {
    auto game_state = GameState::getState();
    unsigned int wave = game_state->getWave();
    auto& field = game_state->getField();
    auto& starting_point = field.getStart();
    auto pos = field.getTile(starting_point).getCenter();

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

void GameManager::start() {
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        Uint32 start = SDL_GetTicks();

        gameLoop();

        if (isKeyPressed(KEY_NSPIRE_F)) { // TODO: Not the right place
            gameLoop();
            gameLoop();
            gameLoop();
        }

        Uint32 stop = SDL_GetTicks();
        Uint32 frame_time = stop - start;
        if (frame_time < STANDARD_TICK_DURATION) {
            SDL_Delay(STANDARD_TICK_DURATION - frame_time);
        }
    }
}

void GameManager::gameLoop() {
    processUserInput();
    auto game_state = GameState::getState();
    IIterable<RReader<Enemy>> enemies = game_state->getEnemies();
    for (auto& enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
        enemy->pathfind(game_state->getField());
    }
    IIterable<RReader<Structure>> structures = game_state->getStructures();
    for (auto& structure = structures.begin(); structure != structures.end(); ++structure) {
        structure->tick();
    }
    handleEnemiesReachingTarget();
    removeDeadEnemies();

    if (field_scene_->visible_) field_scene_->render(screen_);
    if (status_bar_scene_->visible_) status_bar_scene_->render(screen_);

    screen_->flip();
}

void GameManager::handleEnemiesReachingTarget() {
    const auto& state = GameState::getState();

    state->purgeEnemies(
        [&state] (Enemy& e){
            bool target_reached = e.getCenter().x_ > FIELD_WIDTH + 20; // Magic grace number
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

void GameManager::shopLoop() {
}

void GameManager::processUserInput() {
    int actions = Input::getActions();
    if (actions != Input::NONE) {
        field_cursor_->applyUserActions(actions); // TODO: Only if currently in-game

        if ((actions & Input::SPAWN_NEXT_WAVE) && !GameState::getState()->anyEnemiesPresent()) {
            spawnWave();
            GameState::getState()->incrementWave();
        }
    }
}

void GameManager::onMapCursorClickOn(int x, int y) {
    auto game_state = GameState::getState();
    auto& tile = game_state->getField().getTile(x, y);
    TileType type = tile.getType();
    if (TileType::LAND == type) {
        auto structure = game_state->addStructure(std::make_shared<Archer>(24, 15, 1, tile), tile); // TODO: pick class based on store selection
        field_scene_->addToScene(structure);
    } else if (TileType::PATH == type) {
    }
}
