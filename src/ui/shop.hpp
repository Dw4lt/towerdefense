#pragma once
#include "../rendering/abstract_scene.hpp"


class Shop : public AbstractScene {
public:
    static RReader<Shop> create(Screen* screen, SDL_Rect rect, bool visible);

    Shop(Shop&& other) = default;
    Shop& operator=(Shop&& other) = default;

    virtual ~Shop();

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) override;

private:
    /// @note Interaction with screen is necessary for proper initialization, hence create() method.
    Shop(Screen* screen, SDL_Rect rect, bool visible = true);

    // Main font
    nSDL_Font* font_;
};
