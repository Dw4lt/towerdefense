#ifndef RENDERER_H
#define RENDERER_H
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include "renderer_object.hpp"
#include <SDL/SDL_video.h>
#include <map>
#include <vector>
#include <memory>

class Screen;


/// @brief Effectively a subsurface of a Screen. Lifetime owned by Screen.
class Scene {
    using RenderablePtr = RReader<Renderable>;
    using SceneRef = std::unique_ptr<Scene>&;
public:

    Scene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible = true);

    ~Scene();

    /// @brief Add object to the render pipeline of the scene
    void addToScene(RenderablePtr object);

    /// @brief Remove object from the render pipeline of the scene.
    /// @param object Valid object
    void removeFromScene(Renderable* object);

    /// @brief Remove object from the render pipeline of the scene.
    /// @param object Object with irrelevant validity
    void removeFromScene(RenderablePtr object);

    /// @brief Render scene to the screen
    void render(RReader<Screen> screen);

    void setPixel(int x, int y, Uint16 color);
    void fillColor(Rect rect, Uint16 color);
    void drawRect(Rect rect, Uint16 color, Uint8 thickness = 1, Uint8 alpha = SDL_ALPHA_OPAQUE);


    bool visible_;

private:

    /// @brief Ptr to screen this scene is a part of. Valid throughout lifetime.
    Screen* screen_;

    /// @brief Rect describing where the scene is placed relative to the screen
    SDL_Rect rect_on_screen_;

    /// @brief Internal scene surface
    SDL_Surface* surface_;

    /// @brief Objects to render stored in layers
    std::map<SCREEN_LAYER, std::vector<RenderablePtr>> render_objects_; // int -> Layer, vector -> objects
};

#endif
