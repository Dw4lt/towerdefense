#include"field.hpp"
#include"../rendering/renderer.hpp"

Field::Field(int x, int y, int width, int height, int tiles_x, int tiles_y)
    : RendererObject(x, y, width, height)
    , tiles_x_(tiles_x)
    , tiles_y_(tiles_y)
    , tile_grid_((Tile***) malloc(tiles_x * sizeof(Tile**)))
    , start_tile_(0, 0)
{

    // Generate Tile Objects
    double x_increment = ((double)width) / (tiles_x);
    double y_increment = ((double)height) / (tiles_y);
    double x_start = 0;
    double x_stop = x_increment;
    for (int k = 0; k < tiles_x_; k++){
        tile_grid_[k] = (Tile**) calloc(tiles_y_, sizeof(Tile**));
        double y_start = 0;
        double y_stop = y_increment;
        for (int i = 0; i < tiles_y_; i++){
            auto child = new Tile(x_start, y_start, std::round(x_stop-x_start), std::round(y_stop-y_start), k, i);
            children_.push_back(child);
            tile_grid_[k][i] = child;
            y_start = y_stop;
            y_stop += y_increment;
        }
        x_start = x_stop;
        x_stop += x_increment;
    }
    // End of Tile Generation

    generatePath();
}

void Field::generatePath(){
    //Start Point
    int x = 0;
    int y = rand() % tiles_y_;
    tile_grid_[x][y]->updateType(TileType::START);
    start_tile_ = Point{x,y};

    //Path + finish
    bool found = false;
    while (x < tiles_x_){
        found = false;
        while (!found){
            int direction = rand() % 3;
            switch (direction)
            {
            case 0:
                if (y - 2 >= 0 && tile_grid_[x][y-2]->getType() == TileType::GRASS){
                    tile_grid_[x][y]->updateNextNeighbour(Direction::UP);
                    tile_grid_[x][y-1]->updateNextNeighbour(Direction::UP);
                    tile_grid_[x][y-1]->updateType(TileType::PATH);
                    tile_grid_[x][y-2]->updateType(TileType::PATH);
                    y -= 2;
                    found = true;

                }
                break;
            case 1:
                if (y + 2 < tiles_y_ && tile_grid_[x][y+2]->getType() == TileType::GRASS){
                    tile_grid_[x][y]->updateNextNeighbour(Direction::DOWN);
                    tile_grid_[x][y+1]->updateNextNeighbour(Direction::DOWN);
                    tile_grid_[x][y+1]->updateType(TileType::PATH);
                    tile_grid_[x][y+2]->updateType(TileType::PATH);
                    y += 2;
                    found = true;
                }
                break;
            case 2:
                tile_grid_[x][y]->updateNextNeighbour(Direction::RIGHT);
                if (x + 2 < tiles_x_){
                    tile_grid_[x+1][y]->updateType(TileType::PATH);
                    tile_grid_[x+1][y]->updateNextNeighbour(Direction::RIGHT);
                    tile_grid_[x+2][y]->updateType(TileType::PATH);
                }
                else if (x + 1 < tiles_x_) {
                    tile_grid_[x+1][y]->updateType(TileType::FINISH);
                    tile_grid_[x+1][y]->updateNextNeighbour(Direction::RIGHT);
                }
                else {
                    tile_grid_[x][y]->updateType(TileType::FINISH);
                }
                x+= 2;
                found = true;
            default:
                break;
            }
        }
    }

}

Field::~Field(){
    for (int x = 0; x < tiles_x_; x++){
        for (int y = 0; y < tiles_x_; y++){
            delete &tile_grid_[x][y];
        }
    }
    free(tile_grid_);
}

void Field::render(Renderer* renderer){

}

Tile* Field::get(int x, int y){
    if (x >= 0 && x < tiles_x_ && y >= 0 && y < tiles_y_){
        return tile_grid_[x][y];
    }else{
        return nullptr;
    }
}

Tile* Field::get(Point tile_coords){
    return get(tile_coords.x_, tile_coords.y_);
}

int Field::getMaxX() const {
    return tiles_x_ - 1;
}

int Field::getMaxY() const {
    return tiles_y_ - 1;
}

const Point& Field::getStart() const {
    return start_tile_;
}

Point Field::findNextCornerNode(Point coord){
    auto tile = get(coord);
    if (tile){
        Direction dir = tile->getDirectionToNeighbour();
        do{
            switch (dir)
            {
            case Direction::RIGHT:
                coord.x_++;
                break;
            case Direction::UP:
                coord.y_--;
                break;
            case Direction::DOWN:
                coord.y_++;
                break;
            case Direction::LEFT:
                coord.x_--;
                break;
            case Direction::NONE:
                break;
            default:
                break;
            }
            tile = get(coord);
        } while (tile && tile->getDirectionToNeighbour() == dir);
    }
    return coord;
}