#ifndef RENDERER_H
#define RENDERER_H
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include "renderer_object.hpp"
#include <SDL/SDL.h>
#include <map>
#include <vector>


class Renderer {
    using RendererObjectPtr = RReader<RendererObject>;
public:
    static Renderer* Init(int width, int height, int bit_per_color);
    ~Renderer();

    void addToScene(RendererObjectPtr object);
    void removeFromScene(RendererObjectPtr object);
    void show();
    void render();
    void setPixel(int x, int y, Uint16 color);
    void fillColor(Rect rect, Uint16 color);
    // void schedule(RendererObjectPtr object); TODO ???
    void drawRect(Rect rect, Uint16 color, unsigned int thickness = 1, Uint8 alpha = SDL_ALPHA_OPAQUE);

    const int screen_width_;
    const int screen_height_;
    const int screen_bit_color_;
    SDL_Surface* screen_;

    static Renderer* singleton_;

private:
    Renderer(int width, int height, int bit_per_color);

    std::map<SCREEN_LAYER, std::vector<RendererObjectPtr>> render_objects_; // int -> Layer, vector -> objects
};

#endif
