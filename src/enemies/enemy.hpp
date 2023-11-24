#pragma once
#include "../util/constants.hpp"
#include "../rendering/renderer_object.hpp"
#include "../map/field.hpp"
#include <SDL/SDL_video.h>

enum class EnemyType {
    RED = 1,
    BLUE = 2,
    GREEN = 3,
    YELLOW = 4,
    PINK = 5,
    REGROW_RED = 11,
    REGROW_BLUE = 12,
    REGROW_GREEN = 13,
    REGROW_YELLOW = 14,
    REGROW_PINK = 15,
};

class Enemy : public RendererObject {
public:
    Enemy(Point pos, int width, int height, unsigned int target_tile_index, long int hp_, double speed, Uint16 color);
    virtual ~Enemy();
    virtual void render(SDL_Surface* surface) override;

    virtual void tick(const Field& field);

    virtual Rect boundingBox() const override;
    virtual bool isImmune(DAMAGE_TYPE type);
    virtual void damage(int damage, DAMAGE_TYPE type) = 0;

    virtual int getHP() const;
    virtual SCREEN_LAYER getDepth() const override { return SCREEN_LAYER::ENEMY; };

    /// @brief Return index of the tile the enemy is currently standing on. Or -1 if off-screen.
    virtual int getCurrentPathTileIndex() const { return current_path_tile_index_; };

protected:
    /// @brief Run pathfinding logic on the given field
    /// @param field Field to interact with
    virtual void pathfind(const Field& field);

    void updateBoundingBox();

    /// @brief Perform a step towards the given coordinate
    /// @param target_tile Target coordinate
    /// @param distance_to_travel Max distance to travel
    /// @return Target coordinate has been reached within the allowed distance
    bool walkTowards(const Point& target_tile, double& distance_to_travel);

    unsigned int target_path_tile_index_;
    int current_path_tile_index_;
    double real_x_;
    double real_y_;
    Uint16 color_;
    int hp_;
    double speed_;
};
