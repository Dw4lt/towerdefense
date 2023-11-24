#pragma once
#include <SDL/SDL_video.h>
#include "../util/ownership.hpp"

class Screen;

class AbstractScene {
public:

    AbstractScene(AbstractScene&& other) = default;
    AbstractScene& operator=(AbstractScene&& other) = default;

    virtual ~AbstractScene();

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) = 0;

    bool visible_;

protected:
    /// @brief Create scene base. Is not automatically added to screen
    AbstractScene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible = true);

    /// @brief Ptr to screen this scene is a part of. Valid throughout lifetime.
    Screen* screen_;

    /// @brief Rect describing where the scene is placed relative to the screen
    SDL_Rect rect_on_screen_;

    /// @brief Internal scene surface
    SDL_Surface* background_surface_;
};
