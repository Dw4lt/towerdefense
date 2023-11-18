#pragma once
#include "../rendering/abstract_scene.hpp"
#include "../primitives/ownership.hpp"


class Screen;

class StatusBar : public AbstractScene {
public:

    static RReader<StatusBar> create(Screen* screen, SDL_Rect rect, bool visible);

    StatusBar(StatusBar&& other) = default;
    StatusBar& operator=(StatusBar&& other) = default;

    virtual ~StatusBar();

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) override;

private:

    StatusBar(Screen* screen, SDL_Rect rect, bool visible = true);

    // Main font
    nSDL_Font* font_;

    // Re-usable buffers to temporarily store display strings. Required due to right-alignment, width measurement
    char lives_string_[16];
    char money_string_[16];

    // Left edge of icons for icon and text alignment
    Sint16 money_x;
    Sint16 hearts_x;

    // Top edge of text to align with the respective icons
    const int text_y;
};
