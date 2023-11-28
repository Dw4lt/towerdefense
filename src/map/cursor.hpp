#pragma once
#include "../rendering/renderer_object.hpp"
#include "field.hpp"

class GameManager;

typedef enum {
    ON_EMTPY_FIELD,
    ON_STRUCTURE,
    ON_PATH
} ANIMATION_STATE;

class FieldCursor : public RendererObject {
public:
    FieldCursor(GameManager* manager);
    virtual ~FieldCursor();
    virtual void render(SDL_Surface* surface);

    /// @brief Process user input
    /// @param input Bitwise OR-ed actions. Handle action repetition on long button press externally if desired.
    void applyUserActions(int input);

    SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::CURSOR; };

private:
    void updatePosition();
    void updateAnimationState();

    GameManager* game_manager_;
    ANIMATION_STATE animation_state_;

    const unsigned int line_width_;
    int max_x_;
    int max_y_;
    int cursor_x_;
    int cursor_y_;
};
