#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include"rendering/renderer.hpp"
#include"enemies/enemy.hpp"
#include"map/field.hpp"
#include"map/cursor.hpp"
#include <vector>


class GameManager {
public:
    GameManager();
    ~GameManager();
    void start();
private:
    void game_loop();
    void shop_loop();
    void poll();

    Renderer* renderer_;
    Field* field_;
    Cursor* field_cursor_;
    std::vector<Enemy*> enemy_list_;
};

#endif