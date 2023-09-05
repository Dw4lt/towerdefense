#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "enemies/enemy.hpp"
#include "structures/structure.hpp"

using EnemyList = std::vector<Enemy*>;
using StructureList = std::vector<Structure*>;

/// @brief Shared game state singleton class
class GameState {
public:
    /// @brief Deleted Copy-constructor
    GameState(GameState& other) = delete;

    /// @brief Deleted Copy
    void operator=(const GameState&) = delete;

    /// @brief Get global game state singleton
    static GameState* Get();

    /// @brief List of enemies on the field
    EnemyList enemy_list_;

    /// @brief List of sturctures place on the field
    StructureList structure_list_;

private:
    GameState();

    static GameState* singleton_;
};

#endif
