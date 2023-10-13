#ifndef CURSOR_H
#define CURSOR_H

#include "../rendering/renderer_object.hpp"
#include "field.hpp"

class GameManager;

typedef enum {
    ON_EMTPY_FIELD,
    SELECT,
    BLOCKED
} ANIMATION_STATE;

class FieldCursor : public RendererObject {
public:
    FieldCursor(GameManager* manager);
    virtual ~FieldCursor();
    virtual void render(SDL_Surface* surface);

    /// @brief Process user input
    /// @param unfiltered_actions Bitwise OR-ed button states. Long button presses handled internally
    void applyUserActions(int unfiltered_actions);

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

#endif
