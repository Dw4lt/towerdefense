#pragma once
#include "enemies/enemy.hpp"
#include "map/cursor.hpp"
#include "rendering/composable_scene.hpp"
#include "rendering/screen.hpp"
#include "ui/status_bar.hpp"
#include "ui/shop.hpp"
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

    /// @brief Kill enemies that reach the end of the path. Subtract the remaining HP from the lives.
    void handleEnemiesReachingTarget();

    ROwner<Screen> screen_;

    ROwner<FieldCursor> field_cursor_;

    RReader<ComposableScene> field_scene_;

    RReader<StatusBar> status_bar_scene_;

    RReader<Shop> shop_scene_;
};
