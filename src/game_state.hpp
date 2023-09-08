#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "enemies/enemy.hpp"
#include "structures/structure.hpp"
#include "primitives/essentials.hpp"
#include "map/field.hpp"
#include "primitives/ownership.hpp"

using EnemyReadList = std::vector<RReader<Enemy>>;
using StructureList = std::vector<ROwner<Structure>>;


/// @brief Shared game state singleton class
class GameState {
public:
    using GameStatePtr = RReader<GameState>;

    /// @brief Delete Copy-constructor
    GameState(GameState& other) = delete;
    void operator=(const GameState&) = delete;

    ~GameState() = default;

    /// @brief Get global game state singleton
    static GameStatePtr getState();

    Field& getField();

    /// @brief Get RReader iterable of enemies.
    /// @return Iterator over `RReader<Enemy>` view of owned enemies
    RReaderIterable<Enemy> getEnemies() {
        return makeOwnerToReaderWrapper(enemy_list_);
    };

    /// @brief Get RReader iterable of structures.
    /// @return Iterator over `RReader<Structure>` view of owned structures
    RReaderIterable<Structure> getStructures() {
        return makeOwnerToReaderWrapper(structure_list_);
    };

    void addEnemy(std::shared_ptr<Enemy> enemy);

    void addStructure(std::shared_ptr<Structure> structure, Tile& tile);

    /// @brief Destroy enemy instances according to condition
    /// @param func Condition
    void purgeEnemies(bool (*func)(Enemy& e));


    /// @brief List of sturctures place on the field
    StructureList structure_list_;


private:
    GameState();

    /// @brief List of enemies on the field
    std::vector<ROwner<Enemy>> enemy_list_;

    /// @brief Game map, bound in lifetime to the game state.
    ROwner<Field> field_ptr_;

    static ROwner<GameState> singleton_;
};

#endif
