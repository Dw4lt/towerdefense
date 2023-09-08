#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include <SDL/SDL.h>
#include <vector>
#include <memory>

class Renderer;

class Renderable {
public:
    Renderable();
    virtual ~Renderable();

    virtual void render(Renderer* renderer) = 0;

    virtual void renderChildren(Renderer* renderer);

    virtual Rect boundingBox() const = 0;

    void addChild(RReader<Renderable> child);

    bool part_of_a_scene = false;

private:
    std::weak_ptr<Renderable> parent_;
    std::vector<RReader<Renderable>> children_;
};


class RendererObject : public Renderable {
public:
    RendererObject(int x = 0, int y = 0, int width = 1, int height = 1);
    RendererObject(const Rect& rect);
    RendererObject(Point pos, int width, int height);
    virtual ~RendererObject();
    virtual Rect boundingBox() const;
    Uint16 getWidth() const;
    Uint16 getHeight() const;
    virtual void render(Renderer* renderer) = 0;
    virtual void render(Renderer* renderer, const Rect& region);

    virtual SCREEN_LAYER getDepth() const = 0;

    Point getCenter() const;

protected:
    Sint16 x_;
    Sint16 y_;
    Uint16 width_;
    Uint16 height_;
};

#endif
