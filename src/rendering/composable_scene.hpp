#ifndef RENDERER_H
#define RENDERER_H
#include "../primitives/ownership.hpp"
#include "abstract_scene.hpp"
#include "renderer_object.hpp"
#include <SDL/SDL_video.h>
#include <map>
#include <vector>

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


/// @brief Effectively a subsurface of a Screen. Lifetime owned by Screen.
class ComposableScene : public AbstractScene {
public:

    ComposableScene(ComposableScene&& other) = default;
    ComposableScene& operator=(ComposableScene&& other) = default;

    static RReader<ComposableScene> create(Screen* screen, SDL_Rect rect, bool visible = true);

    virtual ~ComposableScene() = default;

    /// @brief Add object to the render pipeline of the scene
    void addToScene(RReader<Renderable> object);

    /// @brief Remove object from the render pipeline of the scene.
    /// @param object Object to remove, validity checked.
    void removeFromScene(RReader<Renderable> object);

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) override;

private:

    /// @brief Create scene. Is not automatically added to screen, hence private.
    ComposableScene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible = true);

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
