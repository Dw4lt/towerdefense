#ifndef SCREEN_H
#define SCREEN_H

#include "abstract_scene.hpp"
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
    RReader<AbstractScene> createScene(SDL_Rect rect);

    /// @brief Get screen surface reference
    SDL_Surface* getSurface() const;

    const ScreenProperties properties_;

    // TODO: Is this really necessary? This seems wrong.
    template <typename T, typename = std::enable_if_t<std::is_base_of<AbstractScene, T>::value>>
    RReader<T> initScene(ROwner<T>&& scene) {
        auto&& owner = std::move(scene);
        auto&& reader = owner.makeReader();
        scenes_.push_back((ROwner<AbstractScene>&&) owner);
        return reader;
    };

    /// @brief Internally used method to create a new surface for a scene, following this instance's scren properties
    /// @return New, unmanaged surface to be used by a scene
    // TODO: This is only used by derivatives of AbstractScene - maybe not public?
    SDL_Surface* createSurface(Uint16 width, Uint16 height) const;

private:


    /// Scenes with managed lifetimes
    std::vector<ROwner<AbstractScene>> scenes_;

    /// Main surface objects get rendered to before SDL_Flip()
    SDL_Surface* surface_;
};

#endif
