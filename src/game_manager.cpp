#include "game_manager.hpp"
#include "structures/archer.hpp"
#include <SDL/SDL.h>
#include <ctime>
#include <iostream>
#include <os.h>
#include <stdio.h>
#include <string>

GameManager::GameManager()
{
    Scene::Init(320, 240, 16);
    GameState::getState();
    field_cursor_ = ROwner(new FieldCursor(this));
    Scene::get()->addToScene(field_cursor_.makeReader());

    spawnWave();
}

void GameManager::spawnWave() {
    auto game_state = GameState::getState();
    auto& field = game_state->getField();
    auto& starting_point = field.getStart();
    auto pos = field.getTile(starting_point).getCenter();
    for (int i = 0; i < 10; i ++) {
        game_state->addEnemy(std::make_shared<Enemy>(pos, 4, 4, Point{starting_point}, 100, 1.2, 0xf00f));
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
        if (any_key_pressed()) {
            poll();
        }
    }
}

void GameManager::gameLoop() {
    field_cursor_->poll();
    auto game_state = GameState::getState();
    for (auto enemy : game_state->getEnemies()) {
        enemy->pathfind(game_state->getField());
    }
    for (auto structure : game_state->getStructures()) {
        structure->tick();
    }
    removeDeadEnemies();
    auto& scene = Scene::get();
    scene->render();
    scene->show();
}

void GameManager::removeDeadEnemies() {
    GameState::getState()->purgeEnemies(
        [](Enemy& e){return e.getHP() <= 0;}
    );
}

void GameManager::shopLoop() {
}

void GameManager::poll() {
}

void GameManager::onMapCursorClickOn(int x, int y) {
    auto game_state = GameState::getState();
    auto& tile = game_state->getField().getTile(x, y);
    TileType type = tile.getType();
    if (TileType::LAND == type) {
        game_state->addStructure(std::make_shared<Archer>(3, 15, 20, tile), tile); // TODO: pick class based on store selection
    } else if (TileType::PATH == type) {
    }
}
