#include"enemy.hpp"
#include"../rendering/renderer.hpp"
#include"../map/field.hpp"
#include <assert.h>
#include<stdio.h>

Enemy::Enemy(Point pos, int width, int height, Point current_field, long int hp, double speed, Uint16 color)
    : RendererObject(pos, width, height)
    , current_field_(current_field)
    , real_x_(pos.x_)
    , real_y_(pos.y_)
    , color_(color)
    , hp_(hp)
    , speed_(speed)
{
}

Enemy::~Enemy(){
    
}

Rect Enemy::boundingBox() const{
    return Rect(x_- width_/2.0, y_ - height_/2.0, width_, height_);
}

void Enemy::render(Renderer* renderer){
    renderer->fill_color(boundingBox(), color_);
}

void Enemy::pathfind(Field* field){
    Tile* tile = field->get(current_field_);
    double distance_to_travel = speed_;
    double x_travel = 0;
    double y_travel = 0;
    if (distance_to_travel > 0){
        Point tile_pos = tile->getCenter();
        Rect current_tile_box = tile->boundingBox();
        double delta_x = tile_pos.x_ - real_x_;
        double delta_y = tile_pos.y_ - real_y_;
        if (delta_x > 0){
            double x_travel = std::min(delta_x, distance_to_travel);
            real_x_ += x_travel;
            x_ = real_x_;
            distance_to_travel -= speed_;
        }
        else{
            switch (tile->getNeighbour())
            {
            case Direction::RIGHT:
                if(current_field_.x_ < field->getMaxX()){
                    x_travel = std::min(distance_to_travel, (current_tile_box.right() - real_x_));
                    printf("Right: %f\n",((double)current_tile_box.right() - real_x_));
                    real_x_ += x_travel;
                    x_ = real_x_;
                    if (distance_to_travel >= x_travel){
                        printf("Going to right tile\n");
                        current_field_.x_++;
                    }
                    distance_to_travel -= x_travel;
                }
                else{
                    // TODO: Reached finish line
                    distance_to_travel = 0;
                }
                break;
            case Direction::UP:
                assert(current_field_.y_ > 0);
                y_travel = std::min(distance_to_travel, (real_y_ - current_tile_box.top()));
                printf("travel: %i\n",y_travel);
                real_y_ -= y_travel;
                distance_to_travel -= y_travel;
                y_ = real_y_;
                if (real_y_ <= current_tile_box.top()){
                    current_field_.y_--;
                }
                break;
            case Direction::DOWN:
                assert(current_field_.y_ < field->getMaxY());
                y_travel = std::min(distance_to_travel, (current_tile_box.bottom() - real_y_));
                printf("travel: %i\n",y_travel);
                real_y_ += y_travel;
                distance_to_travel -= y_travel;
                y_ = real_y_;
                if (real_y_ >= current_tile_box.bottom()){
                    current_field_.y_++;
                }
                break;
            default:
                break;
            }
        }
    }
}