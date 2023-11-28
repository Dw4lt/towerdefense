#include "shop.hpp"
#include "../rendering/screen.hpp"
#include "../util/color.hpp"


Shop::Shop(Screen* screen, SDL_Rect rect, bool visible)
    : AbstractScene(screen, screen->createSurface(rect.w, rect.h), rect, visible)
    , font_{nSDL_LoadFont(NSDL_FONT_VGA, 0xFF, 0xFF, 0xFF)}
{
    // Draw background
    SDL_FillRect(background_surface_, NULL, Colors::SHOP_BACKGROUND);
}

RReader<Shop> Shop::create(Screen* screen, SDL_Rect rect, bool visible) {
    return screen->initScene(ROwner<Shop>(new Shop(screen, rect, visible)));
}

Shop::~Shop() {
    nSDL_FreeFont(font_);
}

void Shop::render(RReader<Screen> screen) {
    auto surface = screen->getSurface();
    SDL_SetClipRect(surface, &rect_on_screen_);
    SDL_BlitSurface(background_surface_, NULL, surface, &rect_on_screen_);

    // TODO: Shop UI

    SDL_SetClipRect(surface, NULL);
}
