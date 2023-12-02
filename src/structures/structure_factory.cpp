#include "structure_factory.hpp"
#include "archer.hpp"
#include "../util/constants.hpp"


ROwner<Tower> StructureFactory::makeTower(TowerType type, int tile_index_x, int tile_index_y) {
    switch (type)
    {
    case TowerType::ARCHER:
        return ROwner<Tower>(new Archer(tile_index_x, tile_index_y));
    default:
        throw "Tower not implemented in factory.";
    }
}
