#pragma once
#include <libndls.h>
#include <SDL/SDL_stdinc.h>

namespace Input
{
    enum Actions {
        NONE = 0,
        ESC = 1,
        SPAWN_NEXT_WAVE = 2,
        LEFT = 4,
        RIGHT = 8,
        UP = 16,
        DOWN = 32,
        CONFIRM = 64,
    };

    /// @brief Get bitwise-OR-ed actions
    int getActions();

    /// @brief Filter out actions which are still being pressed but have not exceded the desired repetition delay
    /// @param previous_timestamp Externally held timestamp of the last time this method was called. Init to 0 is ok. Gets updated upon run.
    /// @param previous_button_state Externally held action state of the last time this method was called. Init to 0 is ok. Gets updated upon run.
    /// @param current_button_state Current action state, as returned by `int getActions()`
    /// @param repetition_timer_ms Duration of time to wait before re-allowing a long-pressed button to trigger a new action
    /// @return Bitwise-OR-ed actions to handle
    int actionRepetitionOnLongPress(Uint32& previous_timestamp, int& previous_button_state, int current_button_state, Uint32 repetition_timer_ms);

} // namespace Input
