#include "status_bar.hpp"
#include "../rendering/screen.hpp"
#include "../primitives/color_conversion.hpp"
#include "../game_state.hpp"


StatusBar::StatusBar(Screen* screen, SDL_Rect rect, bool visible)
    : AbstractScene(screen, screen->createSurface(rect.w, rect.h), std::move(rect), visible)
    , font_{nSDL_LoadFont(NSDL_FONT_VGA, 0, 0, 0)}
{
    DrawUtils::fillColor(background_surface_, Rect(0, 0, rect_on_screen_.w, rect_on_screen_.h), RGB_888_TO_565(0xFFD8AB)); // Main background
    DrawUtils::fillColor(background_surface_, Rect(0, 0, rect_on_screen_.w, 1), RGB_888_TO_565(0x572b0b)); // TOP separation bar
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
