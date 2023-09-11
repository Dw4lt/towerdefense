#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "enemies/enemy.hpp"
#include "map/cursor.hpp"
#include "rendering/scene.hpp"
#include "rendering/screen.hpp"
#include <vector>
#include "game_state.hpp"


class GameManager {
public:
    GameManager();
    ~GameManager() = default;
    void start();
    void onMapCursorClickOn(int x, int y);

    void spawnWave();

private:
    void gameLoop();
    void shopLoop();

    /// @brief Poll and process user's button presses
    void processUserInput();

    void removeDeadEnemies();

    ROwner<Screen> screen_;

    ROwner<FieldCursor> field_cursor_;

    RReader<Scene> field_scene_;
};

#endif
