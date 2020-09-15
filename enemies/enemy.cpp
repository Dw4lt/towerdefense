#include"enemy.hpp"
#include"../rendering/renderer.hpp"

Enemy::Enemy(int x, int y, int width, int height, long int hp, Uint16 color)
    : RendererObject(x, y, width, height)
    , color_(color)
    , hp_(hp)
{
}

void Enemy::render(Renderer* renderer){
    renderer->fill_color(RendererObject::boundingBox(), color_);
}

void Enemy::render(Renderer* renderer, const SDL_Rect& rect){
    renderer->fill_color(RendererObject::boundingBox(), color_);
}