#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <functional>
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

    /// @brief Get a reference to the field instance for immediate use. Do not store.
    Field& getField();

    /// @brief Get a reference to the field instance with validity chek. Safe to store.
    RReader<Field> getFieldReader();

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

    /// @brief Add an enemy to the game state
    /// @param enemy Enemy to take ownership of
    /// @return Reference to managed enemy. Add this to the scene.
    RReader<Enemy> addEnemy(std::shared_ptr<Enemy> enemy);

    /// @brief Check if enemies are present
    bool anyEnemiesPresent() { return enemy_list_.size() > 0; };

    /// @brief Add a structure to the game state
    /// @param structure Structure to take ownership of
    /// @param tile Tile the structure should be assigned to
    /// @return Reference to managed structure. Add this to the scene.
    RReader<Structure> addStructure(std::shared_ptr<Structure> structure, Tile& tile);

    /// @brief Destroy enemy instances according to condition
    /// @param func Condition
    void purgeEnemies(std::function<bool(Enemy&)> func);

    /// @brief Get Nr. of current wave or wave to be spawned
    unsigned int getWave() { return wave_count_; };

    /// @brief Increment Nr. of wave
    /// @return New value
    int incrementWave() { return ++wave_count_; };

    /// @brief Get Nr. of current wave or wave to be spawned
    int getLives() { return lives_; };

    /// @brief Subtract an amount of damage from the current number of lives
    /// @param damage Amount of lives to subtract
    /// @return Remaining lives
    int takeLives(int damage) { lives_ -= damage; return lives_; };

    /// @brief Get amount of money owned by the player
    int getMoney() { return money_; };

    /// @brief Subtract an amount amount of money from the player balance
    /// @param amount Amount of money to subtract
    /// @return Resulting balance
    int takeMoney(int amount) { money_ -= amount; return money_; };

    /// @brief Add an amount amount of money from the player balance
    /// @param amount Amount of money to add
    /// @return Resulting balance
    int addMoney(int amount) { money_ += amount; return money_; };

private:

    GameState();

    /// @brief List of sturctures place on the field
    StructureList structure_list_;

    /// @brief List of enemies on the field
    std::vector<ROwner<Enemy>> enemy_list_;

    /// @brief Game map, bound in lifetime to the game state.
    ROwner<Field> field_ptr_;

    /// @brief Nr. of current wave or wave to be spawned
    unsigned int wave_count_;

    /// @brief Nr. of lives left
    int lives_;

    /// @brief Amount of money the player currently holds
    unsigned int money_;

    static ROwner<GameState> singleton_;
};

#endif
