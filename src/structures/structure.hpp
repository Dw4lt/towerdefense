#pragma once
#include "../map/field.hpp"
#include "../rendering/renderer_object.hpp"

enum class StructureType {
    TOWER,
};

class Structure : public RendererObject {
public:
    Structure(const Rect& bounding_box);

    virtual ~Structure();

    virtual void tick() = 0;

    virtual void onEndOfWave() {};

    virtual StructureType getStructureType() const noexcept = 0;

    virtual void render(SDL_Surface* surface) override;

    SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::STRUCTURE; };
};
