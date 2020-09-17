#include"renderer.hpp"
#include<stdio.h>

Renderer* Renderer::singleton_(nullptr);


Renderer* Renderer::Init(int width, int height, int bit_per_color){
    Renderer::singleton_ = new Renderer(width, height, bit_per_color);
    return Renderer::singleton_;
}

Renderer::Renderer(int width, int height, int bit_per_color)
    : screen_width_(width)
    , screen_height_(height)
    , screen_bit_color_(bit_per_color)
    , screen_(SDL_SetVideoMode(screen_width_, screen_height_, screen_bit_color_, SDL_FULLSCREEN))
{
    
}

void Renderer::addToSchene(RendererObject* object, int layer){
    if (render_objects_.count(layer) == 0){
        render_objects_[layer] = std::vector<RendererObject*>();
    }
    for (RendererObject* child : object->children_){
        addToSchene(child, layer + 1); // Children always on top of parents. This may need changing in the future
    }
    render_objects_[layer].push_back(object);
}

void Renderer::set_pixel(int x, int y, Uint16 color)
{
    if(x >= 0 && y >= 0 && x < screen_width_ && y < screen_height_){
        *((Uint16*) screen_->pixels + y * screen_->pitch + x * screen_bit_color_) = color;
    }
}

void Renderer::fill_color(Rect rect, Uint16 color)
{
    SDL_Rect rect2{rect.toSDLRect()};
    SDL_FillRect(screen_, &rect2, color);
}

void Renderer::render(){
    for (std::pair<int, std::vector<RendererObject*>> vect : render_objects_){
        for (auto obj : vect.second){
            obj->render(this);
        }
    }
}

Renderer::~Renderer(){
    SDL_free(screen_);
}

void Renderer::display(){
    SDL_Flip(screen_);
}