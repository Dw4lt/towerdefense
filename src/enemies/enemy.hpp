#ifndef ENEMY_H
#define ENEMY_H
#include "../primitives/damage.hpp"
#include "../rendering/renderer_object.hpp"
#include "../primitives/essentials.hpp"
#include "../map/field.hpp"
#include <SDL/SDL_video.h>

enum class EnemyType {
    RED = 1,
    BLUE = 2,
    GREEN = 3,
    YELLOW = 4,
    PINK = 5,
};

class Enemy : public RendererObject {
public:
    Enemy(Point pos, int width, int height, unsigned int target_tile_index, long int hp_, double speed, Uint16 color);
    virtual ~Enemy();
    virtual void render(SDL_Surface* surface) override;
    virtual void pathfind(const Field& field);
    virtual Rect boundingBox() const override;
    virtual bool isImmune(DAMAGE_TYPE type);
    virtual void damage(int damage, DAMAGE_TYPE type) = 0;

    virtual int getHP() const;
    virtual SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::ENEMY; };

protected:
    void updateBoundingBox();

    /// @brief Perform a step towards the given coordinate
    /// @param target_tile Target coordinate
    /// @param distance_to_travel Max distance to travel
    /// @return Target coordinate has been reached within the allowed distance
    bool walkTowards(const Point& target_tile, double& distance_to_travel);

    unsigned int target_tile_index_;
    double real_x_;
    double real_y_;
    Uint16 color_;
    int hp_;
    double speed_;
};

#endif
