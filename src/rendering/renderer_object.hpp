#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include <SDL/SDL_video.h>
#include <vector>
#include <memory>


class Scene;

class Renderable {
public:
    Renderable(SCREEN_LAYER layer);

    virtual ~Renderable();

    virtual void render(Scene* scene) = 0;

    virtual void renderChildren(Scene* scene);

    virtual Rect boundingBox() const = 0;

    SCREEN_LAYER getLayer();

    void addChild(RReader<Renderable> child);

    friend class Scene;

private:
    void setScene(Scene* scene);

    std::weak_ptr<Renderable> parent_;
    std::vector<RReader<Renderable>> children_;

    Scene* scene_;
    SCREEN_LAYER layer_;
};


class RendererObject : public Renderable {
public:
    RendererObject(SCREEN_LAYER layer);
    RendererObject(int x, int y, int width, int height, SCREEN_LAYER layer);
    RendererObject(Rect rect, SCREEN_LAYER layer);
    RendererObject(Point pos, int width, int height, SCREEN_LAYER layer);
    virtual ~RendererObject();
    virtual Rect boundingBox() const;
    Uint16 getWidth() const;
    Uint16 getHeight() const;
    virtual void render(Scene* scene) = 0;

    virtual SCREEN_LAYER getDepth() const = 0;

    Point getCenter() const;

protected:
    Rect rect_;
};

#endif
