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


class Tile : public RendererObject {
public:
    Tile(int x, int y, int width, int height, int index_x, int index_y);
    virtual ~Tile() = default;

    void updateType(TileType newType);
    const TileType& getType(){return type;};
    virtual void render(Renderer* renderer) override;
private:
    int index_x_;
    int index_y_;
    Uint16 color_;
    TileType type;
};

#endif