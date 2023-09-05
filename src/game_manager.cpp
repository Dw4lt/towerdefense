#include "game_manager.hpp"
#include "structures/archer.hpp"
#include <SDL/SDL.h>
#include <ctime>
#include <iostream>
#include <os.h>
#include <stdio.h>
#include <string>

GameManager::GameManager()
    : renderer_(Renderer::Init(320, 240, 16))
    , field_(new Field(0, 0, 320, 210, 40, 30))
    , field_cursor_(new Cursor(field_, this))
    , game_state_(GameState::Get())
    {
    renderer_->addToScene(field_, Renderer::LAYER::FIELD_LAYER);
    renderer_->addToScene(field_cursor_, Renderer::LAYER::CURSOR_LAYER);
    game_state_->enemy_list_.push_back(new Enemy(field_->get(field_->getStart())->getCenter(), 4, 4, Point{field_->getStart()}, 100, 4.2, 0xf00f));
    renderer_->addToScene(game_state_->enemy_list_.back(), Renderer::LAYER::ENEMIES_LAYER);
}

GameManager::~GameManager() {
    delete renderer_;
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
    for (Enemy* enemy : game_state_->enemy_list_) {
        enemy->pathfind(field_);
    }
    for (Structure* structure : game_state_->structure_list_) {
        structure->tick();
    }
    renderer_->render();
    renderer_->show();
}

void GameManager::shopLoop() {
}

void GameManager::poll() {
}

void GameManager::onMapCursorClickOn(int x, int y) {
    Tile* tile = field_->get(x, y);
    TileType type = tile->getType();
    if (TileType::LAND == type) {
        auto structure = new Archer(3, 15, 5, tile); // TODO: pick class based on store selection
        tile->addChild(structure);
        tile->updateType(TileType::STRUCTURE);
        game_state_->structure_list_.push_back(structure);
    } else if (TileType::PATH == type) {
    }
}
