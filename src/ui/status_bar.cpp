#include "status_bar.hpp"
#include "../rendering/screen.hpp"
#include "../primitives/color_conversion.hpp"
#include "../game_state.hpp"
#include "../textures/status_bar_background.hpp"
#include "../textures/dollars.hpp"
#include "../textures/hearts.hpp"

StatusBar::StatusBar(Screen* screen, SDL_Rect rect, bool visible)
    : AbstractScene(screen, screen->createSurface(rect.w, rect.h), rect, visible)
    , font_{nSDL_LoadFont(NSDL_FONT_VGA, 0xFF, 0xFF, 0xFF)}
    , text_y{DrawUtils::verticallyCenterFont(font_, rect_on_screen_) + 3}
{
    // Draw background
    {
        auto background = nSDL_LoadImage(image_status_bar_background);
        auto r = SDL_Rect{x: 0, y: 0, w: rect.w, h: rect.h};
        SDL_BlitSurface(background, NULL, background_surface_, &r);
        SDL_FreeSurface(background);
    }

    // Draw hearts icon
    {
        auto hearts = nSDL_LoadImage(image_hearts);
        hearts_x = rect.x + rect.w - hearts->w - 5;
        auto r = SDL_Rect{
            x: hearts_x,
            y: (Sint16) ((rect.h - hearts->h) / 2 + 2),
            w: (Uint16) hearts->w,
            h: (Uint16) hearts->h
        };
        SDL_SetColorKey(hearts, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(hearts->format, 0, 0, 0)); // Allow for alpha in blending
        SDL_BlitSurface(hearts, NULL, background_surface_, &r);
        SDL_FreeSurface(hearts);
    }

    // Draw money icon
    {
        auto dollars = nSDL_LoadImage(image_dollars);
        money_x = rect.x + rect.w - dollars->w - 75;
        auto r = SDL_Rect{
            x: money_x,
            y: (Sint16) ((rect.h - dollars->h) / 2 + 2),
            w: (Uint16) dollars->w,
            h: (Uint16) dollars->h
        };
        SDL_SetColorKey(dollars, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(dollars->format, 0, 0, 0)); // Allow for alpha in blending
        SDL_BlitSurface(dollars, NULL, background_surface_, &r);
        SDL_FreeSurface(dollars);
    }
}

StatusBar::~StatusBar() {
    nSDL_FreeFont(font_);
}

RReader<StatusBar> StatusBar::create(Screen* screen, SDL_Rect rect, bool visible) {
    return screen->initScene(ROwner<StatusBar>(new StatusBar(screen, rect, visible)));
}

void StatusBar::render(RReader<Screen> screen) {
    auto surface = screen->getSurface();
    SDL_SetClipRect(surface, &rect_on_screen_);
    SDL_BlitSurface(background_surface_, NULL, surface, &rect_on_screen_);

    // Wave counter
    nSDL_DrawString(surface, font_, rect_on_screen_.x + 10, text_y, "Round: %i", GameState::getState()->getWave());

    // Lives
    {
        snprintf(lives_string_, sizeof(lives_string_),  "%i", GameState::getState()->getLives());
        auto string_width = nSDL_GetStringWidth(font_, lives_string_);
        nSDL_DrawString(surface, font_, hearts_x - 5 - string_width, text_y, lives_string_);
    }

    // Money
    {
        snprintf(money_string_, sizeof(money_string_),  "%i", GameState::getState()->getMoney());
        auto string_width = nSDL_GetStringWidth(font_, money_string_);
        nSDL_DrawString(surface, font_, money_x - 5 - string_width, text_y, money_string_);
    }
    SDL_SetClipRect(surface, NULL);
}
