#include "cursor.hpp"
#include "../game_manager.hpp"
#include "../primitives/color_conversion.hpp"
#include <os.h>

Cursor::Cursor(const Field* field, GameManager* manager)
    : RendererObject(Point(-1,-1)
                     , field->getWidth() / field->getMaxX() + 2
                     , field->getHeight() / field->getMaxY() + 2)
    , game_manager_(manager)
    , field_{field}
    , line_width_{2}
    , max_x_{field->getMaxX()}
    , max_y_{field->getMaxY()}
    , cursor_x_{max_x_/2}
    , cursor_y_{max_y_/2}
{
    updatePosition();
    updateAnimationState();
}

Cursor::~Cursor() {
}

void Cursor::poll(){
    static bool enable = true;
    static Uint32 last_pass = SDL_GetTicks();
    if (enable == false && SDL_GetTicks() - last_pass >= STANDARD_TICK_DURATION * 2){
        enable = true;
    }
    if (enable && any_key_pressed()){
        bool has_moved = false;
        if ((isKeyPressed(KEY_NSPIRE_4) || isKeyPressed(KEY_NSPIRE_A) || isKeyPressed(KEY_NSPIRE_LEFT)) && cursor_x_ > 0){
            cursor_x_ --;
            has_moved = true;
        }
        else if ((isKeyPressed(KEY_NSPIRE_6) || isKeyPressed(KEY_NSPIRE_D) || isKeyPressed(KEY_NSPIRE_RIGHT)) && cursor_x_ < max_x_){
            cursor_x_ ++;
            has_moved = true;
        }
        if ((isKeyPressed(KEY_NSPIRE_8) || isKeyPressed(KEY_NSPIRE_W) || isKeyPressed(KEY_NSPIRE_UP)) && cursor_y_ > 0){
            cursor_y_ --;
            has_moved = true;
        }
        else if ((isKeyPressed(KEY_NSPIRE_2) || isKeyPressed(KEY_NSPIRE_S) || isKeyPressed(KEY_NSPIRE_DOWN)) && cursor_y_ < max_y_){
            cursor_y_ ++;
            has_moved = true;
        }
        if (isKeyPressed(KEY_NSPIRE_5) || isKeyPressed(KEY_NSPIRE_ENTER)){
            game_manager_->onMapCursorClickOn(cursor_x_, cursor_y_);
            updateAnimationState();
        }

        if (has_moved){
            updatePosition();
            updateAnimationState();
            last_pass = SDL_GetTicks();
            enable = false;
        }
    }
}

void Cursor::updateAnimationState(){
    TileType current_tile_type = field_->get(cursor_x_, cursor_y_)->getType();
    if (TileType::LAND == current_tile_type){
        animation_state_ = ANIMATION_STATE::ON_EMTPY_FIELD;
    }
    else if (TileType::PATH == current_tile_type){
        animation_state_ = ANIMATION_STATE::BLOCKED;
    }
    else if (TileType::STRUCTURE == current_tile_type){
        animation_state_ = ANIMATION_STATE::SELECT;
    }
}

void Cursor::updatePosition(){
    Rect rect = field_->get(cursor_x_, cursor_y_)->boundingBox();
    x_ = rect.left() - 1;
    y_ = rect.top() - 1;
    width_ = rect.width_ + 2;
    height_ = rect.height_ + 2;
}


void Cursor::render(Renderer* renderer){
    Uint32 color;
    switch (animation_state_)
    {
    case ANIMATION_STATE::BLOCKED:
        color = (Uint32) 0xff5900;
        break;
    case ANIMATION_STATE::SELECT:
        color = (Uint32) 0xebd234;
        break;
    case ANIMATION_STATE::ON_EMTPY_FIELD:
    default:
        color = (Uint32) 0x344ceb ;
        break;
    }
    renderer->drawRect(boundingBox(), RGB_888_TO_565(color), 2);
    RendererObject::renderChildren(renderer);
}