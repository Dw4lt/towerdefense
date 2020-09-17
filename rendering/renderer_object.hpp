#ifndef RENDERER_OBJECT
#define RENDERER_OBJECT
#include <SDL/SDL.h>
#include <vector>
#include"../primitives/essentials.hpp"

class Renderer;

class RendererObject {
public:
    RendererObject(int x, int y, int width, int height);
    RendererObject(Point pos, int width, int height);
    virtual ~RendererObject();
    virtual Rect boundingBox() const;
    virtual void render(Renderer* renderer) = 0;
    virtual void render(Renderer* renderer, const Rect& region);
    
    std::vector<RendererObject*> children_;
    Point getCenter();


protected:
    Sint16 x_;
    Sint16 y_;
    Uint16 width_;
    Uint16 height_;
};

#endif