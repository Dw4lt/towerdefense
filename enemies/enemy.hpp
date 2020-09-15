#ifndef ENEMY
#define ENEMY
#include <SDL/SDL.h>
#include "../rendering/renderer_object.hpp"

class Renderer;

class Enemy : public RendererObject {
public:
    Enemy(int x, int y, int width, int height, long int hp_, Uint16 color);
    virtual ~Enemy();
    virtual void render(Renderer* renderer) override;

private:
    Uint16 color_;
    long int hp_;
};

#endif