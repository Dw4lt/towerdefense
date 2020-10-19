#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H
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
    Uint16 getWidth() const;
    Uint16 getHeight() const;
    virtual void render(Renderer* renderer) = 0;
    virtual void render(Renderer* renderer, const Rect& region);
    
    std::vector<RendererObject*> children_;
    Point getCenter() const;


protected:
    Sint16 x_;
    Sint16 y_;
    Uint16 width_;
    Uint16 height_;
};

#endif