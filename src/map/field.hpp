#pragma once
#include "../rendering/renderer_object.hpp"
#include "../primitives/rect.hpp"
#include "../util/ownership.hpp"
#include "tile.hpp"
#include <SDL/SDL_video.h>


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
    inline size_t getPathLength() const noexcept { return path_.size(); };

    void populateTrees();

    SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::BACKGROUND; };

    /// @brief Get the center coordinate of the path-tile with the given index. Used for pathfinding
    /// @param index Index of tile along path
    /// @return Pixel-coordinate of the center of the tile
    Point getPathTileCenter(unsigned int index) const;

    /// @brief Get tile of path at index, or empty RReader if out of bounds.
    RReader<Tile> getPathTile(unsigned int index) const;

    /// @brief Get indexes of all tiles within a certain range of a tile in index coordinates. Furthes tile appears first.
    std::vector<int> getSortedPathTilesWithinRange(int x, int y, int radius) const;

private:
    /// @brief Make the tile at the given index a path
    void addTileToPath(int x, int y);

    int tiles_x_;
    int tiles_y_;

    std::vector<std::vector<ROwner<Tile>>> tile_grid_;
    std::vector<RReader<Tile>> path_;

    Point start_tile_;
};
