#include"field.hpp"
#include"../rendering/renderer.hpp"

Field::Field(int x, int y, int width, int height, int tiles_x, int tiles_y)
    : RendererObject(x, y, width, height)
    , tiles_x_(tiles_x)
    , tiles_y_(tiles_y)
    , tile_grid_((Tile***) malloc(tiles_x * sizeof(Tile**)))
{

   for (int M=0; M<tiles_x; ++M) {
     int at, pre=0, i;
     for (pre = i = 0; i < M; ++i) {
       at = (width+width*i)/M;
       pre = at;
     }
   }


    int x_increment = ((double)width) / (tiles_x);
    int y_increment = ((double)height) / (tiles_y);
    double real_x = 0;
    for (int k = 0; k < tiles_x_; k++){
        double real_y = 0;
        tile_grid_[k] = (Tile**) calloc(tiles_y_, sizeof(Tile**));
        for (int i = 0; i < tiles_y_; i++){
            auto child = new Tile(x + real_x, y + real_y, x_increment, y_increment, k, i);
            children_.push_back(child);
            tile_grid_[k][i] = child;
            real_y += y_increment;
        }
        real_x += x_increment;
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
    if (x > 0 && y < tiles_x_ && y > 0 && y < tiles_y_){
        return tile_grid_[x][y];
    }else{
        return nullptr;
    }
}