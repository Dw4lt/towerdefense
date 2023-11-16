#pragma once
#include "util.hpp"
#include "rect.hpp"

enum SCREEN_LAYER {
    BACKGROUND = 0,
    RESOURCE,
    STRUCTURE,
    ENEMY,
    CURSOR,

    SHOP,
};

static const int STANDARD_TICK_DURATION(41);

static const int FIELD_HEIGHT = 210;
static const int FIELD_WIDTH = 320;
static const int FIELD_TILE_COUNT_X = 40;
static const int FIELD_TILE_COUNT_Y = 30;

static const float BASE_BALOON_SPEED = 1.5;
