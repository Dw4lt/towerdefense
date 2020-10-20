#include "cursor.hpp"
#include "../primitives/color_conversion.hpp"
#include <os.h>

Cursor::Cursor(const Field* field)
    : RendererObject(Point(-1,-1)
                     , field->getWidth() / field->getMaxX() + 2
                     , field->getHeight() / field->getMaxY() + 2)
    , field_{field}
    , line_width_{2}
    , max_x_{field->getMaxX()}
    , max_y_{field->getMaxY()}
{
}

Cursor::~Cursor() {
}

void Cursor::poll(){
    if (any_key_pressed()){
        bool has_moved = false;
        if (isKeyPressed(KEY_NSPIRE_5)){

        }
        else if ((isKeyPressed(KEY_NSPIRE_4) || isKeyPressed(KEY_NSPIRE_A) || isKeyPressed(KEY_NSPIRE_LEFT)) && cursor_x_ > 0){
            cursor_x_ --;
            has_moved = true;
        }
        else if ((isKeyPressed(KEY_NSPIRE_6) || isKeyPressed(KEY_NSPIRE_D) || isKeyPressed(KEY_NSPIRE_RIGHT)) && cursor_x_ < max_x_){
            cursor_x_ ++;
            has_moved = true;
        }
        else if ((isKeyPressed(KEY_NSPIRE_8) || isKeyPressed(KEY_NSPIRE_W) || isKeyPressed(KEY_NSPIRE_UP)) && cursor_y_ > 0){
            cursor_y_ --;
            has_moved = true;
        }
        else if ((isKeyPressed(KEY_NSPIRE_2) || isKeyPressed(KEY_NSPIRE_S) || isKeyPressed(KEY_NSPIRE_DOWN)) && cursor_y_ < max_y_){
            cursor_y_ ++;
            has_moved = true;
        }

        if (has_moved){
            updatePosition();
        }
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
    renderer->drawRect(boundingBox(), RGB_888_TO_565(0xCF2020), 2);
    RendererObject::renderChildren(renderer);
}