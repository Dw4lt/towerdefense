#ifndef RENDERER
#define RENDERER
#include<SDL/SDL.h>
#include<vector>
#include<map>
#include"../primitives/essentials.hpp"
#include"renderer_object.hpp"

class Renderer {
public:
    static Renderer* Init(int width, int height, int bit_per_color);
    ~Renderer();

    void addToSchene(RendererObject* object, int layer);
    void display();
    void render();
    void set_pixel(int x, int y, Uint16 color);
    void fill_color(Rect rect, Uint16 color);
    void schedule(RendererObject* object);
    void draw_rect(Rect rect, Uint16 color, unsigned int thickness = 1, Uint8 alpha = SDL_ALPHA_OPAQUE);

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