#include "game_manager.hpp"
#include <SDL/SDL.h>
#include <stdio.h>
#include <string>
#include<iostream>
#include <ctime>
#include <os.h>



GameManager::GameManager()
    : renderer_(Renderer::Init(320, 240, 16))
    , field_(new Field(0,0,320,210,40,30))
    , field_cursor_(new Cursor(field_))
{
    renderer_->addToSchene(field_, Renderer::LAYER::FIELD_LAYER);
    renderer_->addToSchene(field_cursor_, Renderer::LAYER::CURSOR_LAYER);
    enemy_list_.push_back(new Enemy(field_->get(field_->getStart())->getCenter(), 4, 4, Point{field_->getStart()}, 100, 4.2, 0xf00f));
    renderer_->addToSchene(enemy_list_.back(), Renderer::LAYER::ENEMIES_LAYER);
}

GameManager::~GameManager() {
    delete renderer_;
}

void GameManager::start() {
    while (!isKeyPressed(KEY_NSPIRE_ESC)){
        Uint32 start = SDL_GetTicks();

        game_loop();
        
        Uint32 stop = SDL_GetTicks();
        Uint32 tick_duration = stop - start;
        if (tick_duration < STANDARD_TICK_DURATION){
            SDL_Delay(41-tick_duration);
        }
        if (any_key_pressed()){
            poll();
        }
    }
}

void GameManager::game_loop() {
    field_cursor_->poll();
    for (Enemy* enemy : enemy_list_){
        enemy->pathfind(field_);
    }
    renderer_->render();
    renderer_->display();
}

void GameManager::shop_loop() {

}

void GameManager::poll() {

}
