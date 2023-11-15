#include "field.hpp"
#include "../rendering/composable_scene.hpp"

Field::Field(int x, int y, int width, int height, int tiles_x, int tiles_y)
    : RendererObject(x, y, width, height, SCREEN_LAYER::BACKGROUND)
    , tiles_x_{tiles_x}
    , tiles_y_{tiles_y}
    , start_tile_(0, 0) {

    // Generate Tile Objects
    double x_increment = ((double)width) / (tiles_x);
    double y_increment = ((double)height) / (tiles_y);
    double x_start = 0;
    double x_stop = x_increment;

    tile_grid_.reserve(tiles_x_);

    for (int k = 0; k < tiles_x_; k++) {
        tile_grid_.emplace_back(); // Create nested row-vector
        tile_grid_.back().reserve(tiles_y_);

        double y_start = 0;
        double y_stop = y_increment;
        for (int i = 0; i < tiles_y_; i++) {
            tile_grid_.back().push_back(ROwner(new Tile(x_start, y_start, std::round(x_stop - x_start), std::round(y_stop - y_start), k, i)));
            addChild(tile_grid_.back().back().makeReader());
            y_start = y_stop;
            y_stop += y_increment;
        }
        x_start = x_stop;
        x_stop += x_increment;
    }
    // End of Tile Generation

    generatePath();
    populateTrees();
}

auto Field::getPathTile(unsigned int index) const -> RReader<Tile> {
    if (index < path_.size()) {
        return path_[index];
    }
    return RReader<Tile>();
}

auto Field::getPathTileCenter(unsigned int index) const -> Point {
    if (index < path_.size()) {
        return path_[index]->getCenter();
    }
    return Point(SHRT_MAX, path_.back()->getCenter().y_);
}

auto Field::addTileToPath(int x, int y) -> void {
    auto& t = tile_grid_[x][y];
    t->updateType(TileType::PATH);
    path_.push_back(t.makeReader());
}

auto Field::generatePath() -> void {
    // Start Point
    Sint16 x{0};
    Sint16 y{(Sint16)((rand() % (tiles_y_ / 3)) + tiles_y_ / 3)};

    addTileToPath(x, y);

    start_tile_ = Point(x, y);

    // Path + finish
    bool found = false;
    int max_step = 5;
    int distance = 0;
    const int top_bot_margin = 1; // Margin at the top and at the bottom of the map to avoid paths along the field border
    while (x < tiles_x_) {
        found = false;
        printf("x:%i y:%i\n", x, y);
        while (!found) {
            int direction = rand() % 3;
            switch (direction) {
            case 0: // UP
                if (!(x > 0 && y > top_bot_margin && tile_grid_[x - 1][y - 1]->getType() == TileType::PATH)) {
                    distance = rand() % std::min(y + 1 - top_bot_margin, max_step);
                    for (int i = 0; i < distance && tile_grid_[x][y - 1]->getType() == TileType::LAND; i++) {
                        addTileToPath(x, --y);
                        found = true;
                    }
                }
                break;
            case 1: // DOWN
                if (!(x > 0 && y < tiles_y_ - 1 - top_bot_margin && tile_grid_[x - 1][y + 1]->getType() == TileType::PATH)) {
                    distance = rand() % std::min(tiles_y_ - y - top_bot_margin, max_step);
                    for (int i = 0; i < distance && tile_grid_[x][y + 1]->getType() == TileType::LAND; i++) {
                        addTileToPath(x, ++y);
                        found = true;
                    }
                }
                break;
            case 2: // RIGHT
                distance = rand() % std::min(tiles_x_ - x, max_step);
                for (int i = 0; i < distance && tile_grid_[x + 1][y]->getType() == TileType::LAND; i++) {
                    addTileToPath(++x, y);
                    found = true;
                }
                if (x + 1 == tiles_x_) {
                    x++;
                    found = true;
                }
            default:
                break;
            }
        }
    }
}

auto Field::render(SDL_Surface* surface) -> void {
    renderChildren(surface);
}

auto Field::getTile(int x, int y) -> Tile& {
    if (checkBounds(x, y)) {
        return *tile_grid_[x][y];
    } else {
        throw "Invalid tile index.";
    }
}

auto Field::getTile(const Point& tile_coords) -> Tile& {
    return getTile(tile_coords.x_, tile_coords.y_);
}

auto Field::getTile(int x, int y) const -> const Tile& {
    if (checkBounds(x, y)) {
        return *tile_grid_[x][y];
    } else {
        throw "Invalid tile index.";
    }
}

auto Field::getTile(const Point& tile_coords) const -> const Tile& {
    return getTile(tile_coords.x_, tile_coords.y_);
}

auto Field::checkBounds(int x, int y) const -> bool {
    return x >= 0 && x < tiles_x_ && y >= 0 && y < tiles_y_;
}

auto Field::checkBounds(const Point& tile_coords) const -> bool {
    return checkBounds(tile_coords.x_, tile_coords.y_);
}

auto Field::getMaxX() const -> int {
    return tiles_x_ - 1;
}

auto Field::getMaxY() const -> int {
    return tiles_y_ - 1;
}

auto Field::getStart() const -> const Point& {
    return start_tile_;
}

auto Field::populateTrees() -> void {
    const int radius = 5;
    for (const auto& tile : path_) {
        int x = tile->getIndexX();
        int y = tile->getIndexY();

        for (int dx = -radius; dx < radius; dx++) {
            for (int dy = -radius; dy < radius; dy++) {
                if (!(dx == 0 && dy == 0) && x + dx >= 0 && x + dx < tiles_x_ && y + dy >= 0 && y + dy < tiles_y_ && std::abs(dx * dy) < radius * 1.707) {
                    auto& t = tile_grid_[x + dx][y + dy];
                    if (t->getTerrain() == TileTerrain::TREE) t->updateTerrain(TileTerrain::PLAIN);
                }
            }
        }
    }
}
