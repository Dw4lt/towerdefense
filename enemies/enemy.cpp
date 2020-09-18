#include"enemy.hpp"
#include"../rendering/renderer.hpp"
#include"../map/field.hpp"
#include <assert.h>
#include<stdio.h>

Enemy::Enemy(Point pos, int width, int height, Point current_field, long int hp, double speed, Uint16 color)
    : RendererObject(pos, width, height)
    , current_tile_(current_field)
    , current_target_tile_(current_field)
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

void Enemy::setNextTarget(Field* field){
    current_tile_ = current_target_tile_;
    current_target_tile_ = field->findNextCornerNode(current_tile_);
}

void Enemy::pathfind(Field* field){
    try{
        double distance_to_travel = speed_;
        double x_travel = 0;
        double y_travel = 0;

        while (distance_to_travel > 0){
            Direction dir = Direction::RIGHT;
            Tile* current = field->get(current_tile_);
            if (current != nullptr){
                dir = current->getDirectionToNeighbour();
            }
            Tile* current_destination_tile = field->get(current_target_tile_);
            Point current_destination;
            if (current_destination_tile != nullptr){
                current_destination = current_destination_tile->getCenter();
            } else {
                current_destination = Point{current_tile_.x_, INT_MAX};
            }
            if (dir == Direction::RIGHT || dir == Direction::LEFT){
                x_travel = std::min(std::abs(current_destination.x_ - real_x_), distance_to_travel);
                real_x_ += (dir == Direction::RIGHT) ? x_travel : - x_travel;
                x_ = std::round(real_x_);
                distance_to_travel -= x_travel;
                if (current_destination.x_ <= real_x_){
                    setNextTarget(field);
                }
            } else {
                y_travel = std::min(std::abs(current_destination.y_ - real_y_), distance_to_travel);
                distance_to_travel -= y_travel;
                real_y_ += (dir == Direction::DOWN) ? y_travel : - y_travel;
                if (dir == Direction::DOWN){
                    if (current_destination.y_ <= real_y_) {
                        setNextTarget(field);
                    }
                } else {
                    if (current_destination.y_ >= real_y_) {
                        setNextTarget(field);
                    }
                }
                y_ = std::round(real_y_);
            }
        }
    } catch(...){
        ErrorStream(" x:" << real_x_ << " y:" << real_y_ 
                    << " ct:" << current_tile_.x_ << ";" << current_tile_.y_ 
                    << " nt:"<< current_target_tile_.x_ << ";" << current_target_tile_.y_);
    }
}