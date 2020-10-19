#ifndef RENDERER_H
#define RENDERER_H
#include<SDL/SDL.h>
#include<vector>
#include<map>
#include"../primitives/essentials.hpp"
#include"renderer_object.hpp"


class Renderer {
public:
    typedef enum{
        FIELD_LAYER = 0,
        ENEMIES_LAYER = 10,
        STRUCTURES_LAYER = 15,
        EFFECTS_LAYER = 20,
        CURSOR_LAYER = 50
    } LAYER;

    static Renderer* Init(int width, int height, int bit_per_color);
    ~Renderer();

    void addToSchene(RendererObject* object, int layer);
    void display();
    void render();
    void setPixel(int x, int y, Uint16 color);
    void fillColor(Rect rect, Uint16 color);
    void schedule(RendererObject* object);
    void drawRect(Rect rect, Uint16 color, unsigned int thickness = 1, Uint8 alpha = SDL_ALPHA_OPAQUE);

    const int screen_width_;
    const int screen_height_;
    const int screen_bit_color_;
    SDL_Surface* screen_;

    static Renderer* singleton_;

private:
    Renderer(int width, int height, int bit_per_color);

    std::map<int, std::vector<RendererObject*>> render_objects_; // int -> Layer, vector -> objects
};

#endif