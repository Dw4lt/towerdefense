#ifndef CURSOR_H
#define CURSOR_H

#include "../rendering/renderer.hpp"
#include "../rendering/renderer_object.hpp"
#include "field.hpp"

class GameManager;

typedef enum {
    ON_EMTPY_FIELD,
    SELECT,
    BLOCKED
} ANIMATION_STATE;

class Cursor : public RendererObject {
public:
    Cursor(const Field* field, GameManager* manager);
    virtual ~Cursor();
    virtual void render(Renderer* renderer);

    void poll();

private:
    void updatePosition();
    void updateAnimationState();

    GameManager* game_manager_;
    ANIMATION_STATE animation_state_;

    const Field* field_;
    const unsigned int line_width_;
    int max_x_;
    int max_y_;
    int cursor_x_;
    int cursor_y_;
};

#endif