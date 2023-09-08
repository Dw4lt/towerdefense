#include "cursor.hpp"
#include "../game_manager.hpp"
#include "../primitives/color_conversion.hpp"
#include <os.h>

FieldCursor::FieldCursor(GameManager* manager)
    : RendererObject()
    , game_manager_(manager)
    , line_width_{2}
{
    auto game_state = GameState::getState();
    auto& field = game_state->getField();

    max_x_ = field.getMaxX();
    max_y_ = field.getMaxY();
    cursor_x_ = max_x_ / 2;
    cursor_y_ = max_y_ / 2;

    width_ = field.getWidth() / field.getMaxX() + 2;
    height_ = field.getHeight() / field.getMaxY() + 2;

    updatePosition();
    updateAnimationState();
}

FieldCursor::~FieldCursor() {
}

void FieldCursor::poll() {
    static bool enable = true;
    static Uint32 last_pass = SDL_GetTicks();
    if (enable == false && SDL_GetTicks() - last_pass >= STANDARD_TICK_DURATION * 2) {
        enable = true;
    }
    if (enable && any_key_pressed()) {
        bool has_moved = false;
        if ((isKeyPressed(KEY_NSPIRE_4) || isKeyPressed(KEY_NSPIRE_A) || isKeyPressed(KEY_NSPIRE_LEFT)) && cursor_x_ > 0) {
            cursor_x_--;
            has_moved = true;
        } else if ((isKeyPressed(KEY_NSPIRE_6) || isKeyPressed(KEY_NSPIRE_D) || isKeyPressed(KEY_NSPIRE_RIGHT)) && cursor_x_ < max_x_) {
            cursor_x_++;
            has_moved = true;
        }
        if ((isKeyPressed(KEY_NSPIRE_8) || isKeyPressed(KEY_NSPIRE_W) || isKeyPressed(KEY_NSPIRE_UP)) && cursor_y_ > 0) {
            cursor_y_--;
            has_moved = true;
        } else if ((isKeyPressed(KEY_NSPIRE_2) || isKeyPressed(KEY_NSPIRE_S) || isKeyPressed(KEY_NSPIRE_DOWN)) && cursor_y_ < max_y_) {
            cursor_y_++;
            has_moved = true;
        }
        if (isKeyPressed(KEY_NSPIRE_5) || isKeyPressed(KEY_NSPIRE_ENTER)) {
            game_manager_->onMapCursorClickOn(cursor_x_, cursor_y_);
            updateAnimationState();
        }

        if (has_moved) {
            updatePosition();
            updateAnimationState();
            last_pass = SDL_GetTicks();
            enable = false;
        }
    }
}

void FieldCursor::updateAnimationState() {
    TileType current_tile_type = GameState::getState()->getField().getTile(cursor_x_, cursor_y_).getType();
    if (TileType::LAND == current_tile_type) {
        animation_state_ = ANIMATION_STATE::ON_EMTPY_FIELD;
    } else if (TileType::PATH == current_tile_type) {
        animation_state_ = ANIMATION_STATE::BLOCKED;
    } else if (TileType::STRUCTURE == current_tile_type) {
        animation_state_ = ANIMATION_STATE::SELECT;
    }
}

void FieldCursor::updatePosition() {
    auto& field = GameState::getState()->getField();
    Rect rect = field.getTile(cursor_x_, cursor_y_).boundingBox();
    x_ = rect.left() - 1;
    y_ = rect.top() - 1;
    width_ = rect.width_ + 2;
    height_ = rect.height_ + 2;
}

void FieldCursor::render(Scene* scene) {
    Uint32 color;
    switch (animation_state_) {
    case ANIMATION_STATE::BLOCKED:
        color = (Uint32)0xff5900;
        break;
    case ANIMATION_STATE::SELECT:
        color = (Uint32)0xebd234;
        break;
    case ANIMATION_STATE::ON_EMTPY_FIELD:
    default:
        color = (Uint32)0x344ceb;
        break;
    }
    scene->drawRect(boundingBox(), RGB_888_TO_565(color), 2);
    RendererObject::renderChildren(scene);
}
