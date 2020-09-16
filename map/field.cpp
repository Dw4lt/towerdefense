#include"field.hpp"
#include"../rendering/renderer.hpp"

Field::Field(int x, int y, int width, int height, int tiles_x, int tiles_y)
    : RendererObject(x, y, width, height)
    , tiles_x_(tiles_x)
    , tiles_y_(tiles_y)
    , tile_grid_((Tile***) malloc(tiles_x * sizeof(Tile**)))
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
    if (x > 0 && y < tiles_x_ && y > 0 && y < tiles_y_){
        return tile_grid_[x][y];
    }else{
        return nullptr;
    }
}