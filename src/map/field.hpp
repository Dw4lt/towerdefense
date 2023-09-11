#ifndef MAP_H
#define MAP_H
#include "../rendering/renderer_object.hpp"
#include "../primitives/essentials.hpp"
#include "../primitives/ownership.hpp"
#include "tile.hpp"
#include <SDL/SDL_video.h>

class Scene;


// Game field. Made up of Tiles.
class Field : public RendererObject {
public:
    /// @brief Deleted Copy-constructor. Done to avoid unintentional copying.
    Field(Field& other) = delete;

    /// @brief Deleted Copy. Done to avoid unintentional copying.
    void operator=(const Field& other) = delete;

    Field(int x, int y, int width, int height, int tiles_x = FIELD_TILE_COUNT_X, int tiles_y = FIELD_TILE_COUNT_Y);
    virtual ~Field() = default;
    virtual void render(SDL_Surface* surface) override;

    void generatePath();

    Tile& getTile(int x, int y);
    Tile& getTile(const Point& tile_coords);
    const Tile& getTile(int x, int y) const;
    const Tile& getTile(const Point& tile_coords) const;

    bool checkBounds(int x, int y) const;
    bool checkBounds(const Point& tile_coords) const;

    int getMaxX() const;
    int getMaxY() const;
    const Point& getStart() const;
    Point findNextCornerNode(Point coord) const;

    void populateTrees();

    SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::BACKGROUND; };

private:
    int tiles_x_;
    int tiles_y_;

    std::vector<std::vector<ROwner<Tile>>> tile_grid_;

    Point start_tile_;
};

#endif
