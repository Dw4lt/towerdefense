#include "game_manager.hpp"
#include "primitives/input.hpp"
#include "structures/archer.hpp"
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

    field_scene_ = screen_->createScene(SDL_Rect{x: 0, y: 0, w: FIELD_WIDTH, h: FIELD_HEIGHT});

    field_scene_->addToScene(game_state->getFieldReader());

    field_scene_->addToScene(field_cursor_.makeReader());

    spawnWave();
}

void GameManager::spawnWave() {
    auto game_state = GameState::getState();
    auto& field = game_state->getField();
    auto& starting_point = field.getStart();
    auto pos = field.getTile(starting_point).getCenter();
    for (int i = 0; i < 10; i ++) {
        auto enemy = game_state->addEnemy(std::make_shared<Enemy>(pos, 4, 4, Point{starting_point}, 100, 1.2, 0xf00f));
        field_scene_->addToScene(enemy);
        pos.x_ -= 15;
    }
}

void GameManager::start() {
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        Uint32 start = SDL_GetTicks();

        gameLoop();

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
    for (auto enemy : game_state->getEnemies()) {
        enemy->pathfind(game_state->getField());
    }
    for (auto structure : game_state->getStructures()) {
        structure->tick();
    }
    removeDeadEnemies();

    if (field_scene_->visible_) field_scene_->render(screen_);

    screen_->flip();
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

        if (actions & Input::SPAWN_NEXT_WAVE) {
            spawnWave();
        }
    }
}

void GameManager::onMapCursorClickOn(int x, int y) {
    auto game_state = GameState::getState();
    auto& tile = game_state->getField().getTile(x, y);
    TileType type = tile.getType();
    if (TileType::LAND == type) {
        auto structure = game_state->addStructure(std::make_shared<Archer>(3, 15, 20, tile), tile); // TODO: pick class based on store selection
        field_scene_->addToScene(structure);
    } else if (TileType::PATH == type) {
    }
}
