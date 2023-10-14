#include "status_bar.hpp"
#include "../rendering/screen.hpp"
#include "../primitives/color_conversion.hpp"
#include "../game_state.hpp"
#include "../textures/status_bar_background.h"


StatusBar::StatusBar(Screen* screen, SDL_Rect rect, bool visible)
    : AbstractScene(screen, screen->createSurface(rect.w, rect.h), rect, visible)
    , font_{nSDL_LoadFont(NSDL_FONT_VGA, 0xFF, 0xFF, 0xFF)}
{
    // Draw background
    auto background = nSDL_LoadImage(status_bar_background);
    auto r = SDL_Rect{x: 0, y:0, w: rect.w, h: rect.h};
    SDL_BlitSurface(background, NULL, background_surface_, &r);
    SDL_FreeSurface(background);
}

StatusBar::~StatusBar() {
    nSDL_FreeFont(font_);
}

RReader<StatusBar> StatusBar::create(Screen* screen, SDL_Rect rect, bool visible) {
    return screen->initScene(ROwner<StatusBar>(new StatusBar(screen, rect, visible)));
}

void StatusBar::render(RReader<Screen> screen) {
    auto surface = screen->getSurface();
    SDL_BlitSurface(background_surface_, NULL, surface, &rect_on_screen_);

    int center_aligned_y = DrawUtils::verticallyCenterFont(font_, rect_on_screen_);

    // Wave counter
    nSDL_DrawString(surface, font_, rect_on_screen_.x + 10, center_aligned_y, "Wave: %i", GameState::getState()->getWave());

    // Lives
    snprintf(lives_string_, sizeof(lives_string_),  "HP: %i", GameState::getState()->getLives());
    auto string_width = nSDL_GetStringWidth(font_, lives_string_);
    nSDL_DrawString(surface, font_, rect_on_screen_.x + rect_on_screen_.w - 10 - string_width, center_aligned_y, lives_string_);
}
