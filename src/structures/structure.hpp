#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "../map/field.hpp"
#include "../primitives/color_conversion.hpp"
#include "../primitives/essentials.hpp"
#include "../rendering/renderer_object.hpp"

class Structure : public RendererObject {
public:
    Structure(Tile* tile);
    virtual ~Structure();

    virtual void tick() = 0;

    virtual void render(Renderer* renderer);

protected:
    Tile* tile_;
};

#endif
