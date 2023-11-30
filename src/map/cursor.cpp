#include "cursor.hpp"
#include "../game_manager.hpp"
#include "../util/color.hpp"
#include "../input.hpp"
#include "../primitives/rect.hpp"
#include "../structures/tower.hpp"

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

    RendererObject::setWidth(field.getWidth() / field.getMaxX() + 2);
    RendererObject::setHeight(field.getHeight() / field.getMaxY() + 2);

    updatePosition();
    updateAnimationState();
}

FieldCursor::~FieldCursor() {
}

void FieldCursor::applyUserActions(int input) {
    bool has_moved = false;

    if (input & Input::CONFIRM) {
        game_manager_->onMapCursorClickOn(cursor_x_, cursor_y_);
        updateAnimationState();
    }

    if ((input & Input::LEFT) && cursor_x_ > 0) {
        cursor_x_--;
        has_moved = true;
    } else if ((input & Input::RIGHT) && cursor_x_ < max_x_) {
        cursor_x_++;
        has_moved = true;
    }
    if ((input & Input::UP) && cursor_y_ > 0) {
        cursor_y_--;
        has_moved = true;
    } else if ((input & Input::DOWN) && cursor_y_ < max_y_) {
        cursor_y_++;
        has_moved = true;
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
        animation_state_ = ANIMATION_STATE::ON_PATH;
    } else if (TileType::STRUCTURE == current_tile_type) {
        animation_state_ = ANIMATION_STATE::ON_STRUCTURE;
    }
}

void FieldCursor::updatePosition() {
    Point center = GameState::getState()->getField().getTile(cursor_x_, cursor_y_).boundingBox().center();
    RendererObject::centerOn(center.x_, center.y_);
}

void FieldCursor::render(SDL_Surface* surface) {
    Uint32 color;
    switch (animation_state_) {
    case ANIMATION_STATE::ON_PATH:
        color = Colors::CURSOR_ON_PATH;
        break;
    case ANIMATION_STATE::ON_STRUCTURE:
        color = Colors::CURSOR_ON_BUILDING;
        {
            // Show tower range on hover
            auto structure = GameState::getState()->getStructure(cursor_x_, cursor_y_).get();
            switch (structure->getStructureType()) {
                case StructureType::TOWER:
                {
                    Tower* tower = static_cast<Tower*>(structure);
                    int range = tower->getRange();
                    if (range > 0) {
                        Point center = RendererObject::center();
                        DrawUtils::drawPixelatedCircleOutline(surface, center.x_, center.y_, range, FIELD_TILE_WIDTH, FIELD_TILE_HEIGHT, Colors::RED, 1);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        break;
    case ANIMATION_STATE::ON_EMTPY_FIELD:
    default:
        color = Colors::CURSOR_ON_EMPTY_FIELD_TILE;
        break;
    }
    DrawUtils::drawRect(surface, boundingBox(), color, 2);
    RendererObject::renderChildren(surface);
}
