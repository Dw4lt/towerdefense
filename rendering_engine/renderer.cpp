#include"renderer.hpp"


Renderer* Init(int width, int height, int bit_per_color){

}

Renderer::Renderer(int width, int height, int bit_per_color)
    : screen_width_(width)
    , screen_height_(height)
    , screen_bit_color_(int bit_per_color)
    , screen_(SDL_SetVideoMode(screen_width_, screen_height_, screen_bit_color_, SDL_FULLSCREEN))
{
    
}

void Renderer::render(){
    for (auto obj : render_objects_)
}

Renderer::~Renderer(){
    SDL_Free(screen_);
}

Renderer::display(){
    SDL_Flip(screen_);
}