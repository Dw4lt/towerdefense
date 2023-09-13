#ifndef TILE_H
#define TILE_H
#include "../rendering/renderer_object.hpp"
#include <SDL/SDL_video.h>


enum class TileType {
    LAND,
    PATH,
    STRUCTURE
};

typedef enum {
    TREE,
    ROCK,
    PLAIN
} TileTerrain;

typedef enum {
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
    void updateNextNeighbour(Point point);
    void updateTerrain(TileTerrain terrain);
    const TileType& getType() const { return type_; };
    const Point& getNextNeighbour() const { return next_neighbour_; };
    virtual void render(SDL_Surface* surface) override;

    SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::BACKGROUND; };

private:
    void updateColor();

    int index_x_;
    int index_y_;
    Uint16 color_;
    TileType type_;
    Point next_neighbour_;
    TileTerrain terrain_;
    int distance_to_water_;
};

#endif
