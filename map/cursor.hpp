#ifndef CURSOR
#define CURSOR

#include "field.hpp"
#include "../rendering/renderer_object.hpp"
#include "../rendering/renderer.hpp"

class Cursor : public RendererObject {
public:
    Cursor(const Field* field);
    virtual ~Cursor();
    virtual void render(Renderer* renderer);

    void poll();

private:
    void updatePosition();
    
    const Field* field_;
    const unsigned int line_width_;
    int cursor_x_;
    int cursor_y_;
    int max_x_;
    int max_y_;
};

#endif