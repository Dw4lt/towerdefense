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
    int y = (rand() % (tiles_y_ / 3)) + tiles_y_ / 3;
    tile_grid_[x][y]->updateType(TileType::PATH);
    start_tile_ = Point{x,y};

    //Path + finish
    bool found = false;
    int max_step = 4;
    int distance = 0;
    while (x < tiles_x_){
        SDL_Delay(3);
        found = false;
        printf("x:%i y:%i\n",x, y);
        while (!found){
            int direction = rand() % 3;
            switch (direction)
            {
            case 0: // UP
                if (!(x > 0 && y > 1 && tile_grid_[x-1][y-1]->getType()==TileType::PATH)){
                    distance = rand() % std::min(y+1, max_step);
                    for (int i = 0; i < distance && tile_grid_[x][y-1]->getType() == TileType::LAND; i++){
                        tile_grid_[x][y]->updateNextNeighbour(Direction::UP);
                        tile_grid_[x][--y]->updateType(TileType::PATH);
                        found = true;
                    }
                }
                break;
            case 1: // DOWN
                if (!(x > 0 && y < tiles_y_ -1 && tile_grid_[x-1][y+1]->getType()==TileType::PATH)){
                    distance = rand() % std::min(tiles_y_ - y, max_step);
                    for (int i = 0; i < distance && tile_grid_[x][y+1]->getType() == TileType::LAND; i++){
                        tile_grid_[x][y]->updateNextNeighbour(Direction::DOWN);
                        tile_grid_[x][++y]->updateType(TileType::PATH);
                        found = true;
                    }
                }
                break;
            case 2: // RIGHT
                distance = rand() % std::min(tiles_x_ - x, max_step);
                for (int i = 0; i < distance && tile_grid_[x+1][y]->getType() == TileType::LAND; i++){
                    tile_grid_[x][y]->updateNextNeighbour(Direction::RIGHT);
                    tile_grid_[++x][y]->updateType(TileType::PATH);
                    found = true;
                }
                if (x + 1 == tiles_x_){
                    tile_grid_[x][y]->updateNextNeighbour(Direction::RIGHT);
                    x++;
                    found = true;
                }
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