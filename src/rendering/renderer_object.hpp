#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include <SDL/SDL_video.h>
#include <vector>
#include <memory>


class ComposableScene;

namespace DrawUtils {
    void setPixel(SDL_Surface* surface, int x, int y, Uint16 color);
    void fillColor(SDL_Surface* surface, Rect rect, Uint16 color);
    void drawLine(SDL_Surface* surface, float x1, float y1, float x2, float y2, const Uint16 color);
    void drawRect(SDL_Surface* surface, Rect rect, Uint16 color, Uint8 thickness = 1);
    int verticallyCenterFont(nSDL_Font* font, const SDL_Rect& rect);
};

class Renderable {
public:
    Renderable(SCREEN_LAYER layer);

    virtual ~Renderable();

    virtual void render(SDL_Surface* surface) = 0;

    virtual void renderChildren(SDL_Surface* surface);

    virtual Rect boundingBox() const = 0;

    SCREEN_LAYER getLayer() { return layer_; };

    void addChild(RReader<Renderable> child);

    ComposableScene* getScene() {return scene_; };

    friend class ComposableScene; // TODO: hacky. But maybe appropriate? Questionmark?

private:

    void setScene(ComposableScene* scene) { scene_ = scene; };

    std::weak_ptr<Renderable> parent_;
    std::vector<RReader<Renderable>> children_;

    ComposableScene* scene_;
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
    virtual void render(SDL_Surface* surface) = 0;

    virtual SCREEN_LAYER getDepth() const = 0;

    Point getCenter() const;

protected:
    Rect rect_;
};

#endif
