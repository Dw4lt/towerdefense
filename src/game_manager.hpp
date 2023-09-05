#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "enemies/enemy.hpp"
#include "map/cursor.hpp"
#include "map/field.hpp"
#include "rendering/renderer.hpp"
#include "structures/structure.hpp"
#include <vector>

class GameManager {
public:
    GameManager();
    ~GameManager();
    void start();
    void onMapCursorClickOn(int x, int y);

private:
    void gameLoop();
    void shopLoop();
    void poll();

    Renderer* renderer_;
    Field* field_;
    Cursor* field_cursor_;
    std::vector<Enemy*> enemy_list_;
};

#endif