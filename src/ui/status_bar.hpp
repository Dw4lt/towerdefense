#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "../rendering/scene.hpp"
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
};

#endif // STATUS_BAR_H
