#ifndef RENDERER_OBJECT
#define RENDERER_OBJECT
#include <SDL/SDL.h>
#include <vector>

class Renderer;

class RendererObject {
public:
    RendererObject(int x, int y, int width, int height);
    virtual ~RendererObject();
    SDL_Rect boundingBox();
    virtual void render(Renderer* renderer) = 0;
    virtual void render(Renderer* renderer, const SDL_Rect& region);
    
    std::vector<RendererObject*> children_;

private:
    Sint16 x_;
    Sint16 y_;
    Uint16 width_;
    Uint16 height_;
};

#endif