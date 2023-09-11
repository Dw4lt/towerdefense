#ifndef INPUT_H
#define INPUT_H

#include <libndls.h>

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

    /// @brief Get bitwise-OR-ed state of actions
    int getActions();

} // namespace Input

#endif // INPUT_H
