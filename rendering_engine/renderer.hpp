#ifndef RENDERER
#define RENDERER
#include<SDL/SDL.h>
#include<vector>
#include<map>
class RenderObject;

class Renderer {
public:
    Renderer* Init(int width, int height, int bit_per_color);
    ~Renderer();

    void render();
    void display();
    void addToSchene(RenderObject* object);
    void schedule(RenderObject* object);

    const int screen_width_;
    const int screen_height_;
    const int screen_bit_color_;
    SDL_Surface* screen_;

    static Renderer* renderer_instance;

private:
    Renderer(int width, int height, int bit_per_color);

    std::vector<RenderObject*> render_objects_;
    std::map<RenderObject*, SDL_Rect> scheduled_render_objects_;
};

#endif