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

/// @brief Vector of weakly-held renderable objects with dirty flag
struct SceneLayer : public std::vector<RReader<Renderable>> {

    SceneLayer();

    /// @brief Avoid accidental copy bugs
    SceneLayer(const SceneLayer& other) = delete;

    /// @brief Avoid accidental copy bugs
    SceneLayer& operator=(SceneLayer&& other) = default;

    /// @brief Avoid accidental copy bugs
    SceneLayer& operator=(const SceneLayer& other) = delete;

    /// @brief Mark layer as is in need of re-rendering
    void markDirty(bool dirty = true) { dirty_ = dirty; };

    /// @brief Layer is in need of re-rendering
    bool isDirty() { return dirty_; };

private:
    /// @brief Layer is in need of re-rendering
    bool dirty_ = true;
};


class AbstractScene {
public:

    AbstractScene(AbstractScene&& other) = default;
    AbstractScene& operator=(AbstractScene&& other) = default;

    virtual ~AbstractScene();

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) = 0;

    bool visible_;

protected:
    /// @brief Create scene base. Is not automatically added to screen
    AbstractScene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible = true);

    /// @brief Ptr to screen this scene is a part of. Valid throughout lifetime.
    Screen* screen_;

    /// @brief Rect describing where the scene is placed relative to the screen
    SDL_Rect rect_on_screen_;

    /// @brief Internal scene surface
    SDL_Surface* background_surface_;
};


/// @brief Effectively a subsurface of a Screen. Lifetime owned by Screen.
class Scene : public AbstractScene {
    using RenderablePtr = RReader<Renderable>;
    using SceneRef = std::unique_ptr<Scene>&;
public:

    Scene(Scene&& other) = default;
    Scene& operator=(Scene&& other) = default;

    static RReader<Scene> create(Screen* screen, SDL_Rect rect, bool visible = true);

    virtual ~Scene() = default;

    /// @brief Add object to the render pipeline of the scene
    void addToScene(RenderablePtr object);

    /// @brief Remove object from the render pipeline of the scene.
    /// @param object Valid object
    void removeFromScene(Renderable* object);

    /// @brief Remove object from the render pipeline of the scene.
    /// @param object Object with irrelevant validity
    void removeFromScene(RenderablePtr object);

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) override;

private:

    /// @brief Create scene. Is not automatically added to screen, hence private.
    Scene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible = true);

    /// @brief Get layer if present
    /// @return Layer or nullptr
    SceneLayer* getLayer(SCREEN_LAYER layer);

    /// @brief Render beckground layer to cached surface if dirty
    void renderBackgroundIfNecessary();

    /// @brief Render all objects of layer to surface
    void renderLayer(SceneLayer& layer, SDL_Surface* surface);

    /// @brief Objects to render stored in layers
    std::map<SCREEN_LAYER, SceneLayer> render_objects_; // int -> Layer, vector -> objects
};

#endif
