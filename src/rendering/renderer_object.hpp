#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include <SDL/SDL.h>
#include <vector>
#include <memory>

class Scene;

class Renderable {
public:
    Renderable();
    virtual ~Renderable();

    virtual void render(Scene* scene) = 0;

    virtual void renderChildren(Scene* scene);

    virtual Rect boundingBox() const = 0;

    void addChild(RReader<Renderable> child);

    void inheritScene();

    bool part_of_a_scene = false;

private:
    std::weak_ptr<Renderable> parent_;
    std::vector<RReader<Renderable>> children_;
};


class RendererObject : public Renderable {
public:
    RendererObject();
    RendererObject(int x, int y, int width, int height);
    RendererObject(Rect rect);
    RendererObject(Point pos, int width, int height);
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
