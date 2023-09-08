#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "enemies/enemy.hpp"
#include "map/cursor.hpp"
#include "rendering/renderer.hpp"
#include <vector>
#include "game_state.hpp"

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
    void removeDeadEnemies();

    Renderer* renderer_;
    ROwner<FieldCursor> field_cursor_;
};

#endif
