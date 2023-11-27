#pragma once
#include "enemies/enemy.hpp"
#include "map/cursor.hpp"
#include "rendering/composable_scene.hpp"
#include "rendering/screen.hpp"
#include "ui/status_bar.hpp"
#include "ui/shop.hpp"
#include "util/event.hpp"
#include <vector>

class UserInputEvent;

class GameManager {
public:
    GameManager();
    ~GameManager() = default;

    void onMapCursorClickOn(int x, int y);

    void spawnWave();
    void endWave();

    void mainGameLoop();
    void mainRenderLoop();

    void shopRenderLoop();

    void setShopVisibility(bool visible) { shop_scene_->visible_ = visible; };
    void setFieldVisibility(bool visible) { field_scene_->visible_ = visible; };

    /// @brief Wrapper to pass on input to internal field cursor
    void applyUserInputToFieldCursor(UserInputEvent& event);

private:
    void removeDeadEnemies();

    /// @brief Kill enemies that reach the end of the path. Subtract the remaining HP from the lives.
    void handleEnemiesReachingTarget();

    ROwner<Screen> screen_;

    ROwner<FieldCursor> field_cursor_;

    RReader<ComposableScene> field_scene_;

    RReader<StatusBar> status_bar_scene_;

    RReader<Shop> shop_scene_;
};
