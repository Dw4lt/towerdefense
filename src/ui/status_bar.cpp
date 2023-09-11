#include "status_bar.hpp"
#include "../rendering/screen.hpp"
#include "../primitives/color_conversion.hpp"
#include "../game_state.hpp"


StatusBar::StatusBar(Screen* screen, SDL_Rect rect, bool visible)
    : AbstractScene(screen, screen->createSurface(rect.w, rect.h), std::move(rect), visible)
    , font_{nSDL_LoadFont(NSDL_FONT_VGA, 0, 0, 0)}
{
    DrawUtils::fillColor(background_surface_, Rect(0, 0, rect_on_screen_.w, rect_on_screen_.h), RGB_888_TO_565(0xFFD8AB));
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

    int y = DrawUtils::verticallyCenterFont(font_, rect_on_screen_);
    nSDL_DrawString(surface, font_, rect_on_screen_.x + 10, y, "%i", GameState::getState()->getWave());
}
