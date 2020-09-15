#ifndef RENDERER_OBJECT
#define RENDERER_OBJECT
#include <SDL/SDL.h>

class Renderer;

class RendererObject {
public:
    RendererObject(Renderer* renderer, int x, int y, int width, int height);
    ~RendererObject();
    SDL_Rect boundingBox();
    virtual void render() = 0;
    virtual void render(const SDL_Rect& region) = 0;
    
    static const Renderer* renderer;

private:
    Uint16 width_;
    Uint16 height_;
    Sint16 x_;
    Sint16 y_;
};

#endif