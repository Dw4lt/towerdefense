#ifndef TILE
#define TILE
#include <SDL/SDL.h>
#include "../rendering/renderer_object.hpp"

class Renderer;

typedef enum {
    LAND,
    PATH
} TileType;

typedef enum {
    TREE,
    ROCK,
    PLAIN
} TileTerrain;

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
    void updateDistanceToWater(unsigned int distance);
    void updateTerrain(TileTerrain terrain);
    const TileType& getType(){return type_;};
    const Direction& getDirectionToNeighbour(){return next_neighbour_;};
    virtual void render(Renderer* renderer) override;
private:
    void updateColor();

    int index_x_;
    int index_y_;
    Uint16 color_;
    TileType type_;
    Direction next_neighbour_;
    TileTerrain terrain_;
    int distance_to_water_;
};

#endif