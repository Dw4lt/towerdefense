#pragma once
#include <functional>
#include <map>
#include <vector>
#include <deque>

#include "enemies/enemy.hpp"
#include "structures/structure.hpp"
#include "primitives/essentials.hpp"
#include "map/field.hpp"
#include "primitives/ownership.hpp"

using EnemyReadList = std::vector<RReader<Enemy>>;
using StructuresContainer = std::map<unsigned int, ROwner<Structure>>;

enum class WaveState {
    BETWEEN_WAVES,
    ACTIVE_WAVE,
    PAUSED // TODO: implement or handle externally
};


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
    IIterable<RReader<Enemy>> getEnemies() {
        return makeOwnerToReaderWrapper(enemy_list_);
    };

    /// @brief Get RReader iterable of structures.
    /// @return Iterator over `RReader<Structure>` view of owned structures
    IIterable<RReader<Structure>> getStructures() {
        return makeOwnerToReaderWrapper(structures_);
    };

    /// @brief Get structure at given coordinate, if any
    /// @param index_x Tile X coordinate
    /// @param index_y Tile y coordinate
    /// @return Structure or empty
    RReader<Structure> getStructure(int index_x, int index_y);

    /// @brief Add an enemy to the game state
    /// @param enemy Enemy to take ownership of
    /// @return Reference to managed enemy. Add this to the scene.
    RReader<Enemy> addEnemy(std::shared_ptr<Enemy> enemy);

    /// @brief Check if enemies are present
    bool anyEnemiesPresent() { return enemy_list_.size() > 0; };

    /// @brief Get all enemies currently located on the tile behind the given index
    /// @param path_tile_index Index of tile to query
    /// @return Reference to enemies container
    const std::deque<RReader<Enemy>>& getEnemiesOnTile(unsigned int path_tile_index) {
        return tile_enemy_mapping_[path_tile_index]; // Creates if not found.
    };

    /// @brief Transfer enemy from one tile to another, with bounds check.
    /// @param enemy Enemy to move
    /// @param previous_tile_index Previous tile index or -1 if not already on a tile
    /// @param current_tile_index New tile to move to, or -1 to remove from field
    void updateEnemyTile(const RReader<Enemy>& enemy, int previous_tile_index, int current_tile_index);

    /// @brief Add a structure to the game state
    /// @param structure Structure to take ownership of
    /// @param tile Tile the structure should be assigned to
    /// @return Reference to managed structure. Add this to the scene.
    RReader<Structure> addStructure(std::shared_ptr<Structure> structure, Tile& tile);

    /// @brief Destroy enemy instances according to condition
    /// @param func Condition
    void purgeEnemies(std::function<bool(Enemy&)> func);

    /// @brief Get Nr. of currently active wave or recently completed wave
    unsigned int getWave() { return wave_count_; };

    /// @brief Get the current state of the wave
    WaveState getWaveState();

    /// @brief Increment Nr. of wave, declare wave as started
    /// @return Nr. of new wave
    unsigned int startNextWave();

    /// @brief Pay out end-of-wave bonus, declare wave as completed
    void endWave();

    /// @brief Get Nr. of current wave or wave to be spawned
    int getLives() { return lives_; };

    /// @brief Subtract an amount of damage from the current number of lives
    /// @param damage Amount of lives to subtract
    /// @return Remaining lives
    int takeLives(int damage) { lives_ -= damage; return lives_; };

    /// @brief Get amount of money owned by the player
    int getMoney() { return money_; };

    /// @brief Subtract an amount amount of money from the player balance if they have enough
    /// @param amount Amount of money to subtract
    /// @return Whether the transaction was successful
    bool tryTakeMoney(int amount);

    /// @brief Add an amount amount of money from the player balance
    /// @param amount Amount of money to add
    /// @return Resulting balance
    int addMoney(int amount) { money_ += amount; return money_; };

private:

    /// @brief Singleton constructor
    GameState();

    /// @brief Remove enemy from its current tile, thereby removing it from the field.
    void removeEnemyFromField(const RReader<Enemy>& enemy);

    /// @brief List of sturctures place on the field
    StructuresContainer structures_;

    /// @brief List of enemies on the field
    std::vector<ROwner<Enemy>> enemy_list_;

    /// @brief Game map, bound in lifetime to the game state.
    ROwner<Field> field_ptr_;

    /// @brief Index of path tile mapped to enemies on said tile.
    std::vector<std::deque<RReader<Enemy>>> tile_enemy_mapping_;

    /// @brief Nr. of current wave or wave to be spawned
    unsigned int wave_count_;

    /// @brief Nr. of lives left
    int lives_;

    /// @brief Amount of money the player currently holds
    int money_;

    static ROwner<GameState> singleton_;

    WaveState wave_state_;
};
