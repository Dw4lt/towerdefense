#ifndef SCREEN_H
#define SCREEN_H

#include "scene.hpp"
#include <SDL/SDL_video.h>
#include <SDL/SDL_stdinc.h>

struct ScreenProperties {
    /// @brief Bits per pixel, duh.
    const Uint32 bits_per_pixel;

    /// @brief Bitwise mask for blue component of pixels
    const Uint32 B_mask;

    /// @brief Bitwise mask for green component of pixels
    const Uint32 G_mask;

    /// @brief Bitwise mask for red component of pixels
    const Uint32 R_mask;
};

class Screen {
public:

    Screen(int width, int height);

    ~Screen();

    /// @ref SDL_Flip
    int flip() const;

    /// @brief Create a new managed scene
    /// @param rect Size and position of the scene, relative to the screen
    /// @return Scene with managed lifetime
    RReader<Scene> createScene(SDL_Rect rect);

    /// @brief Get screen surface reference
    SDL_Surface* getSurface() const;

    const ScreenProperties properties_;

private:
    /// @brief Create a new surface for a scene, following this instance's scren properties
    /// @return New, unmanaged surface to be used by a scene
    SDL_Surface* createSurface(Uint16 width, Uint16 height) const;

    /// Scenes with managed lifetimes
    std::vector<ROwner<Scene>> scenes_;

    /// Main surface objects get rendered to before SDL_Flip()
    SDL_Surface* surface_;
};

#endif
