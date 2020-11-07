#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "../primitives/essentials.hpp"
#include "../primitives/color_conversion.hpp"
#include "../rendering/renderer_object.hpp"
#include "../map/field.hpp"

class Structure : public RendererObject {
public:
    Structure(Tile* tile);
    virtual ~Structure();

    virtual void render(Renderer* renderer);
protected:
    Tile* tile_;
};

#endif