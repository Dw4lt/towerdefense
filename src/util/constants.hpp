#pragma once
#include <math.h>

enum SCREEN_LAYER {
    BACKGROUND = 0,
    RESOURCE,
    STRUCTURE,
    ENEMY,
    CURSOR,
};

typedef enum {
    PROJECTILE,
    POISON,
    FROST,
    FIRE
} DAMAGE_TYPE;

static const int TICKS_PER_SECOND(24);
static const int STANDARD_TICK_DURATION((int)std::round(1000.0 / TICKS_PER_SECOND));

static const int FIELD_HEIGHT = 210;
static const int FIELD_WIDTH = 320;
static const int FIELD_TILE_COUNT_X = 40;
static const int FIELD_TILE_COUNT_Y = 30;
static const float FIELD_TILE_WIDTH = (float) FIELD_WIDTH / FIELD_TILE_COUNT_X;
static const float FIELD_TILE_HEIGHT = (float) FIELD_HEIGHT / FIELD_TILE_COUNT_Y;

/// @brief Nr of pixels spanning a logical distance of 1 in coordinate space
static const float PIXEL_DISTANCE_RATIO_X = (float) FIELD_TILE_WIDTH / 2;
static const float PIXEL_DISTANCE_RATIO_Y = (float) FIELD_TILE_HEIGHT / 2;

static const float BASE_BALOON_SPEED = 1.5;
