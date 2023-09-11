#include "screen.hpp"


Screen::Screen(int width, int height)
    : properties_{
        bits_per_pixel: 16,
         B_mask: 0x00'1F,
         G_mask: 0x07'E0,
         R_mask: 0xF8'00,
    }
    , surface_(SDL_SetVideoMode(width, height, has_colors ? 16 : 8, SDL_SWSURFACE))
{
    if (surface_ == NULL) {
        throw std::runtime_error("Screen could not allocate surface.");
    }
}

Screen::~Screen() {
    SDL_FreeSurface(surface_);
    surface_ = nullptr;
}

auto Screen::createSurface(Uint16 width, Uint16 height) const -> SDL_Surface* {
    auto surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 16, properties_.R_mask, properties_.G_mask, properties_.B_mask, 0);
    if (surface == NULL) {
        throw std::runtime_error("Screen could not allocate surface.");
    }
    return surface;
}

auto Screen::getSurface() const -> SDL_Surface* {
    return surface_;
}

auto Screen::flip() const -> int {
    return SDL_Flip(surface_);
}
