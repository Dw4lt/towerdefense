#pragma once
#include "../util/ownership.hpp"
#include "structure.hpp"
#include "tower.hpp"


struct StructureFactory {

    /// @brief Create a tower on the given tile
    static ROwner<Tower> makeTower(TowerType type, int tile_index_x, int tile_index_y);
};
