#ifndef TILE
#define TILE
#include <SDL/SDL.h>
#include "../rendering/renderer_object.hpp"

class Renderer;

typedef enum {
    GRASS,
    PATH,
    FILLED_GRASS,
    START,
    FINISH
} TileType;

typedef enum{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
} Direction;


class Tile : public RendererObject {
public:
    Tile(int x, int y, int width, int height, int index_x, int index_y);
    virtual ~Tile() = default;

    void updateType(TileType newType);
    void updateNextNeighbour(Direction direction);
    const TileType& getType(){return type;};
    const Direction& getDirectionToNeighbour(){return next_neighbour_;};
    virtual void render(Renderer* renderer) override;
private:
    int index_x_;
    int index_y_;
    Uint16 color_;
    TileType type;
    Direction next_neighbour_;
};

#endif