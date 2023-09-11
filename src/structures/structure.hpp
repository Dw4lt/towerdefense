#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "../map/field.hpp"
#include "../primitives/color_conversion.hpp"
#include "../primitives/essentials.hpp"
#include "../rendering/renderer_object.hpp"

class Structure : public RendererObject {
public:
    Structure(const Tile& tile);

    virtual ~Structure();

    virtual void tick() = 0;

    virtual void render(SDL_Surface* surface) override;

    SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::STRUCTURE; };
};

#endif
