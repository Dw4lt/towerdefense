#ifndef MAP
#define MAP
#include <SDL/SDL.h>
#include "../rendering/renderer_object.hpp"
#include "tile.hpp"

class Renderer;

class Field : public RendererObject {
public:
    Field(int x, int y, int width, int height, int tiles_x, int tiles_y);
    ~Field();
    virtual void render(Renderer* renderer) override;
    
    void generatePath();
    Tile* get(int x, int y);

private:
    int tiles_x_;
    int tiles_y_;
    Tile*** tile_grid_;
    Point start_tile_;
};

#endif