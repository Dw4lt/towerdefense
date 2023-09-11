#include "scene.hpp"
#include "screen.hpp"
#include <stdio.h>

SceneLayer::SceneLayer()
    : std::vector<RReader<Renderable>>()
{
}

Scene::Scene(Screen* screen, SDL_Surface* surface, SDL_Rect rect, bool visible)
    : visible_(visible)
    , screen_(screen)
    , rect_on_screen_(rect)
    , background_surface_(surface)
{
}

void Scene::addToScene(RenderablePtr object) {
    if (object->scene_) {
        throw std::runtime_error("RendererObject is already part of a scene.");
    }
    auto layer = object->layer_;
    if (render_objects_.count(layer) == 0) {
        render_objects_[layer] = SceneLayer();
    }
    object->setScene(this);
    object->scene_ = this;
    auto& scene_layer = render_objects_[layer];
    scene_layer.push_back(object);
    scene_layer.markDirty();
}

void Scene::removeFromScene(Renderable* object) {
}

void Scene::removeFromScene(RenderablePtr object) {
    if (object.isValid()) {
        auto layer = getLayer(object->layer_);
        if (layer) {
            auto& objects = *layer;
            int size = objects.size();
            for (int i = 0; i < size; i++) {
                if (objects[i].isValid() && objects[i].get() == object.get()) {
                    objects.erase(objects.begin() + i);
                    size--;
                    objects.markDirty();
                }
            }
        }
    }
}

auto Scene::getLayer(SCREEN_LAYER layer) -> SceneLayer* {
    auto l = render_objects_.find(layer);
    if (l != render_objects_.end()) {
        return &(*l).second;
    }
    return nullptr;
}

void Scene::renderBackgroundIfNecessary() {
    auto bg = getLayer(BACKGROUND);
    if (bg && bg->isDirty()) {
        renderLayer(*bg, background_surface_);
        bg->markDirty(false);
    }
}

void Scene::render(RReader<Screen> screen) {
    renderBackgroundIfNecessary();

    // Blit static background
    SDL_BlitSurface(background_surface_, NULL, screen->getSurface(), &rect_on_screen_);

    std::vector<RendererObject> v;
    for (auto& vect : render_objects_) {
        if (vect.first != BACKGROUND) renderLayer(vect.second, screen->getSurface()); // Render directly to screen
    }
}

void Scene::renderLayer(SceneLayer& layer, SDL_Surface* surface){
    for (auto iter = layer.begin(); iter != layer.end();) {
        if ((*iter).isValid()) {
            (*iter)->render(surface);
            iter++;
        } else {
            iter = layer.erase(iter);
        }
    }
}

Scene::~Scene() {
    SDL_free(background_surface_);
}
