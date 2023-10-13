#include "cursor.hpp"
#include "../game_manager.hpp"
#include "../primitives/color_conversion.hpp"
#include "../primitives/input.hpp"
#include <SDL/SDL_timer.h>
#include <os.h>

FieldCursor::FieldCursor(GameManager* manager)
    : RendererObject(SCREEN_LAYER::CURSOR)
    , game_manager_(manager)
    , line_width_{2}
{
    auto game_state = GameState::getState();
    auto& field = game_state->getField();

    max_x_ = field.getMaxX();
    max_y_ = field.getMaxY();
    cursor_x_ = max_x_ / 2;
    cursor_y_ = max_y_ / 2;

    rect_.width_ = field.getWidth() / field.getMaxX() + 2;
    rect_.height_ = field.getHeight() / field.getMaxY() + 2;

    updatePosition();
    updateAnimationState();
}

FieldCursor::~FieldCursor() {
}

void FieldCursor::applyUserActions(int unfiltered_actions) {
    bool has_moved = false;

    // Filter long-press actions to repeat upon delay
    static Uint32 previous_timestamp{0};
    static int previous_button_state{0};
    int actions = Input::actionRepetitionOnLongPress(previous_timestamp, previous_button_state, unfiltered_actions, 3 * STANDARD_TICK_DURATION);

    if ((actions & Input::LEFT) && cursor_x_ > 0) {
        cursor_x_--;
        has_moved = true;
    } else if ((actions & Input::RIGHT) && cursor_x_ < max_x_) {
        cursor_x_++;
        has_moved = true;
    }
    if ((actions & Input::UP) && cursor_y_ > 0) {
        cursor_y_--;
        has_moved = true;
    } else if ((actions & Input::DOWN) && cursor_y_ < max_y_) {
        cursor_y_++;
        has_moved = true;
    }
    if (actions & Input::CONFIRM) {
        game_manager_->onMapCursorClickOn(cursor_x_, cursor_y_);
        updateAnimationState();
    }

    if (has_moved) {
        updatePosition();
        updateAnimationState();
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
    Rect rect = GameState::getState()->getField().getTile(cursor_x_, cursor_y_).boundingBox();
    rect_ = Rect(rect.left() - 1, rect.top() - 1, rect.width_ + 2, rect.height_ + 2);
}

void FieldCursor::render(SDL_Surface* surface) {
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
    DrawUtils::drawRect(surface, boundingBox(), RGB_888_TO_565(color), 2);
    RendererObject::renderChildren(surface);
}
